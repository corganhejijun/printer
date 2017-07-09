using System.Collections;
using System.Threading;

namespace Wpf3DPrint.Viewer
{
    class SceneThread
    {
        private Thread d3DThread;
        private static Mutex threadMutex;
        private static bool stopThread = false;
        private static ArrayList workList;
        private static bool busy;

        public delegate object function(object arg);
        public delegate void afterFunction(object result);
        public delegate void onFunction(object result);

        public bool IsBusy
        {
            get { return busy; }
        }

        private class ThreadFunction
        {
            public function func;
            public afterFunction afterFunc;
            public ArrayList argList;
        }

        public SceneThread()
        {
            d3DThread = new Thread(threadWork);
            workList = new ArrayList();
            threadMutex = new Mutex();
            d3DThread.Start();
            busy = false;
        }

        public void stop()
        {
            threadMutex.WaitOne();
            stopThread = true;
            threadMutex.ReleaseMutex();
        }

        static afterFunction afterRelease;

        public void Dispose(afterFunction release)
        {
            stopThread = true;
            afterRelease = release;
        }

        public void addWork(function func, ArrayList args = null, afterFunction afterFunc = null)
        {
            ThreadFunction threadFunc = new ThreadFunction();
            threadFunc.func = func;
            threadFunc.afterFunc = afterFunc;
            if (null == args)
            {
                workList.Add(threadFunc);
                return;
            }
            threadFunc.argList = new ArrayList();
            foreach (object item in args)
            {
                threadFunc.argList.Add(item);
            }
            workList.Add(threadFunc);
        }

        public static void threadWork()
        {
            while (true)
            {
                ArrayList localList = new ArrayList();
                threadMutex.WaitOne();
                busy = true;
                foreach (ThreadFunction item in workList)
                {
                    localList.Add(item);
                }
                workList.Clear();
                threadMutex.ReleaseMutex();

                localList.Reverse();
                foreach (ThreadFunction item in localList)
                {
                    object result = item.func(item.argList);
                    if (item.afterFunc != null)
                    {
                        item.afterFunc(result);
                    }
                }

                Thread.Sleep(10);

                threadMutex.WaitOne();
                if (stopThread)
                {
                    threadMutex.ReleaseMutex();
                    if (afterRelease != null)
                        afterRelease(null);
                    break;
                }
                busy = false;
                threadMutex.ReleaseMutex();
            }
        }
    }
}

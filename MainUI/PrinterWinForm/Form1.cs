using System;
using System.Windows.Forms;
using System.Collections;

using ManagedExchange;
using Driver;

namespace PrinterWinForm
{
    public partial class Form1 : Form
    {
        class Speed
        {
            public int x, y;
            public int accX, accY;
            public Speed()
            {
                x = y = accX = accY = 0;
            }
        }
        IntPtr param;
        IntPtr param2D;
        ART m_art;
        Speed m_speed;
        ArrayList layerList;

        public Form1()
        {
            InitializeComponent();
            param = ClassDisplay.glInit(panelDisplay.Handle);
            param2D = ClassDisplay.glInit(panel2D.Handle);
            m_art = new ART();
            m_speed = new Speed();
            layerList = new ArrayList();
        }

        private void buttonConnectPCI_Click(object sender, EventArgs e)
        {
            try
            {
                if (m_art.create())
                {
                    MessageBox.Show("连接成功");
                }
                else
                {
                    MessageBox.Show("连接失败");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void panelDisplay_Resize(object sender, EventArgs e)
        {
            ClassDisplay.resize(panelDisplay.Handle, param);
            ClassDisplay.drawEntity(panelDisplay.Handle, param);
            if (panel2D.Visible)
            {
                ClassDisplay.resize(panel2D.Handle, param2D);
                ClassDisplay.drawEntity(panel2D.Handle, param2D);
            }
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            ClassDisplay.glDispose(param);
            ClassDisplay.glDispose(param2D);
        }

        private void toolStripMenuItemOpenFile_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            if (dlg.ShowDialog() != DialogResult.OK)
                return;
            Console.WriteLine(dlg.FileName);
            ClassDisplay.displayDxf(param, dlg.FileName);
            ClassDisplay.resize(panelDisplay.Handle, param);
            ClassDisplay.drawEntity(panelDisplay.Handle, param);
            ClassDisplay.set2DView(param, param2D);
            if (panel2D.Visible)
            {
                ClassDisplay.resize(panel2D.Handle, param2D);
                ClassDisplay.drawEntity(panel2D.Handle, param2D);
            }
            layerList.Clear();
            int layerCount = ClassDisplay.getLayerCount(param);
            for (int i = 0; i < layerCount; i++)
            {
                int number = ClassDisplay.getEntCount(param, i);
                layerList.Add(number);
            }
        }

        private void buttonSpeed_Click(object sender, EventArgs e)
        {
            int speed;
            if (!int.TryParse(textBoxSpeed.Text, out speed))
            {
                MessageBox.Show("速度格式错误");
                return;
            }
            int acc;
            if (!int.TryParse(textBoxAccelerate.Text, out acc))
            {
                MessageBox.Show("加速度格式错误");
                return;
            }
            if (radioButtonX.Checked) {
                m_speed.x = speed;
                m_speed.accX = acc;
            }
            if (radioButtonY.Checked)
            {
                m_speed.y = speed;
                m_speed.accY = acc;
            }
        }

        private void radioButtonX_CheckedChanged(object sender, EventArgs e)
        {
            if (radioButtonX.Checked)
            {
                textBoxAccelerate.Text = m_speed.accX.ToString();
                textBoxSpeed.Text = m_speed.x.ToString();
            }
            if (radioButtonY.Checked)
            {
                textBoxSpeed.Text = m_speed.y.ToString();
                textBoxAccelerate.Text = m_speed.accY.ToString();
            }
        }

        private void buttonX_Click(object sender, EventArgs e)
        {
            if (!m_art.DeviceAvailable)
                return;
            if (m_art.XIsRuning)
            {
                buttonX.Text = "X开始";
                if (!m_art.stopX())
                    MessageBox.Show("停止错误");
            }
            else
            {
                ART.Direction dir = ART.Direction.forward;
                if (checkBoxReverse.Checked)
                    dir = ART.Direction.backward;
                if(!m_art.startX(m_speed.x, m_speed.accX, dir))
                    MessageBox.Show("开始失败");
                else
                    buttonX.Text = "X停止";
            }
        }

        private void buttonY_Click(object sender, EventArgs e)
        {
            if (!m_art.DeviceAvailable)
                return;
            if (m_art.YIsRunning)
            {
                buttonY.Text = "Y开始";
                if(!m_art.stopY())
                    MessageBox.Show("停止错误");
            }
            else
            {
                ART.Direction dir = ART.Direction.forward;
                if (checkBoxReverse.Checked)
                    dir = ART.Direction.backward;
                if (!m_art.startY(m_speed.y, m_speed.accY, dir))
                    MessageBox.Show("开始失败");
                else
                    buttonY.Text = "Y停止";
            }
        }

        private void buttonOptic_Click(object sender, EventArgs e)
        {
            if (!m_art.DeviceAvailable)
                return;

        }

        private void buttonDisp2D_Click(object sender, EventArgs e)
        {
            panel2D.Visible = !panel2D.Visible;
            if (panel2D.Visible)
                buttonDisp2D.Text = "关闭2D视图";
            else
                buttonDisp2D.Text = "显示2D视图";
        }

        bool beginManufact = false;
        int currentLayer = 0;
        int currentEnt = 0;
        int prevMs = 0;
        float prevX = 0;
        float prevY = 0;

        private void manufactStop()
        {
            beginManufact = false;
            buttonManufact.Text = "制造开始";
            timerManufact.Stop();
        }

        private void buttonManufact_Click(object sender, EventArgs e)
        {
            if (beginManufact)
            {
                manufactStop();
            }
            else
            {
                buttonManufact.Text = "制造停止";
                currentLayer = 0;
                currentEnt = 0;
                beginManufact = true;
                timerManufact.Start();
            }
            //ClassDisplay.slowDrawEntity(panel2D.Handle, param2D, 1000);
        }

        int tickCount = 0;
        private void timerManufact_Tick(object sender, EventArgs e)
        {
            tickCount++;
            if (tickCount < numericUpDownManufactSpeed.Value)
            {
                return;
            }
            tickCount = 0;
            if (currentEnt == 0)
            {
                prevX = prevY = 0;
                prevMs = 0;
            }
            int pm = prevMs;
            float px = prevX;
            float py = prevY;
            ClassDisplay.draw1Step(panelDisplay.Handle, param, currentEnt, currentLayer, ref pm, ref px, ref py);
            ClassDisplay.draw1Step(panel2D.Handle, param2D, currentEnt, currentLayer, ref prevMs, ref prevX, ref prevY);
            currentEnt++;
            if (currentEnt >= (int)layerList[currentLayer])
            {
                currentEnt = 0;
                currentLayer++;
                if (currentLayer == layerList.Count)
                {
                    manufactStop();
                }
            }
        }
    }
}

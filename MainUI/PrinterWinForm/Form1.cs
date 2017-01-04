using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

using ManagedExchange;
using System.Runtime.InteropServices;

namespace PrinterWinForm
{
    public partial class Form1 : Form
    {
        IntPtr param;
        public Form1()
        {
            InitializeComponent();
            param = ClassDisplay.glInit(panelDisplay.Handle);
        }

        private void buttonOpenFile_Click(object sender, EventArgs e)
        {
        }

        private void panelDisplay_Paint(object sender, PaintEventArgs e)
        {
            //ClassDisplay.paint(panelDisplay.Handle, param);
            //ClassDisplay.drawEntity(panelDisplay.Handle, param);
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
        }

        private void panelDisplay_Resize(object sender, EventArgs e)
        {
            ClassDisplay.resize(panelDisplay.Handle, param);
            ClassDisplay.drawEntity(panelDisplay.Handle, param);
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            ClassDisplay.glDispose(param);
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
        }
    }
}

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
        ART m_art;
        Speed m_speed;
        public Form1()
        {
            InitializeComponent();
            param = ClassDisplay.glInit(panelDisplay.Handle);
            m_art = new ART();
            m_speed = new Speed();
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

        }

        private void buttonY_Click(object sender, EventArgs e)
        {

        }

        private void buttonOptic_Click(object sender, EventArgs e)
        {

        }

        private void buttonManufact_Click(object sender, EventArgs e)
        {
            ClassDisplay.drawEntity(panelDisplay.Handle, param);
        }
    }
}

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
        IntPtr param2D;
        ART m_art;
        Speed m_speed;
        public Form1()
        {
            InitializeComponent();
            param = ClassDisplay.glInit(panelDisplay.Handle);
            param2D = ClassDisplay.glInit(panel2D.Handle);
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

        private void buttonManufact_Click(object sender, EventArgs e)
        {
            ClassDisplay.slowDrawEntity(panelDisplay.Handle, param, 100);
            if (panel2D.Visible)
                ClassDisplay.slowDrawEntity(panel2D.Handle, param2D, 100);
        }

        private void buttonDisp2D_Click(object sender, EventArgs e)
        {
            panel2D.Visible = !panel2D.Visible;
            if (panel2D.Visible)
                buttonDisp2D.Text = "关闭2D视图";
            else
                buttonDisp2D.Text = "显示2D视图";
        }
    }
}

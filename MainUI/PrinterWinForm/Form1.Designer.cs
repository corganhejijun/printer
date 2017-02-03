namespace PrinterWinForm
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.buttonConnectPCI = new System.Windows.Forms.Button();
            this.menuStrip = new System.Windows.Forms.MenuStrip();
            this.ToolStripMenuItemFile = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItemOpenFile = new System.Windows.Forms.ToolStripMenuItem();
            this.panelDisplay = new System.Windows.Forms.Panel();
            this.statusStrip = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.buttonSpeed = new System.Windows.Forms.Button();
            this.label2 = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.textBoxAccelerate = new System.Windows.Forms.TextBox();
            this.textBoxSpeed = new System.Windows.Forms.TextBox();
            this.radioButtonY = new System.Windows.Forms.RadioButton();
            this.radioButtonX = new System.Windows.Forms.RadioButton();
            this.buttonX = new System.Windows.Forms.Button();
            this.buttonY = new System.Windows.Forms.Button();
            this.buttonOptic = new System.Windows.Forms.Button();
            this.checkBoxReverse = new System.Windows.Forms.CheckBox();
            this.buttonManufact = new System.Windows.Forms.Button();
            this.buttonDisp2D = new System.Windows.Forms.Button();
            this.panel2D = new System.Windows.Forms.Panel();
            this.timerManufact = new System.Windows.Forms.Timer(this.components);
            this.numericUpDownManufactSpeed = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.menuStrip.SuspendLayout();
            this.statusStrip.SuspendLayout();
            this.groupBox1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownManufactSpeed)).BeginInit();
            this.SuspendLayout();
            // 
            // buttonConnectPCI
            // 
            this.buttonConnectPCI.Location = new System.Drawing.Point(0, 28);
            this.buttonConnectPCI.Name = "buttonConnectPCI";
            this.buttonConnectPCI.Size = new System.Drawing.Size(75, 23);
            this.buttonConnectPCI.TabIndex = 0;
            this.buttonConnectPCI.Text = "连接设备";
            this.buttonConnectPCI.UseVisualStyleBackColor = true;
            this.buttonConnectPCI.Click += new System.EventHandler(this.buttonConnectPCI_Click);
            // 
            // menuStrip
            // 
            this.menuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.ToolStripMenuItemFile});
            this.menuStrip.Location = new System.Drawing.Point(0, 0);
            this.menuStrip.Name = "menuStrip";
            this.menuStrip.Size = new System.Drawing.Size(597, 25);
            this.menuStrip.TabIndex = 1;
            this.menuStrip.Text = "menuStrip1";
            // 
            // ToolStripMenuItemFile
            // 
            this.ToolStripMenuItemFile.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItemOpenFile});
            this.ToolStripMenuItemFile.Name = "ToolStripMenuItemFile";
            this.ToolStripMenuItemFile.Size = new System.Drawing.Size(58, 21);
            this.ToolStripMenuItemFile.Text = "文件(&F)";
            // 
            // toolStripMenuItemOpenFile
            // 
            this.toolStripMenuItemOpenFile.Name = "toolStripMenuItemOpenFile";
            this.toolStripMenuItemOpenFile.Size = new System.Drawing.Size(127, 22);
            this.toolStripMenuItemOpenFile.Text = "打开(&O)...";
            this.toolStripMenuItemOpenFile.Click += new System.EventHandler(this.toolStripMenuItemOpenFile_Click);
            // 
            // panelDisplay
            // 
            this.panelDisplay.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panelDisplay.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.panelDisplay.Location = new System.Drawing.Point(81, 28);
            this.panelDisplay.Name = "panelDisplay";
            this.panelDisplay.Size = new System.Drawing.Size(516, 417);
            this.panelDisplay.TabIndex = 2;
            this.panelDisplay.Resize += new System.EventHandler(this.panelDisplay_Resize);
            // 
            // statusStrip
            // 
            this.statusStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip.Location = new System.Drawing.Point(0, 448);
            this.statusStrip.Name = "statusStrip";
            this.statusStrip.Size = new System.Drawing.Size(597, 22);
            this.statusStrip.TabIndex = 3;
            this.statusStrip.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(131, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.buttonSpeed);
            this.groupBox1.Controls.Add(this.label2);
            this.groupBox1.Controls.Add(this.label1);
            this.groupBox1.Controls.Add(this.textBoxAccelerate);
            this.groupBox1.Controls.Add(this.textBoxSpeed);
            this.groupBox1.Controls.Add(this.radioButtonY);
            this.groupBox1.Controls.Add(this.radioButtonX);
            this.groupBox1.Location = new System.Drawing.Point(0, 57);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(75, 171);
            this.groupBox1.TabIndex = 4;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "groupBox1";
            // 
            // buttonSpeed
            // 
            this.buttonSpeed.Location = new System.Drawing.Point(6, 142);
            this.buttonSpeed.Name = "buttonSpeed";
            this.buttonSpeed.Size = new System.Drawing.Size(63, 23);
            this.buttonSpeed.TabIndex = 6;
            this.buttonSpeed.Text = "应用";
            this.buttonSpeed.UseVisualStyleBackColor = true;
            this.buttonSpeed.Click += new System.EventHandler(this.buttonSpeed_Click);
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(4, 100);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(71, 12);
            this.label2.TabIndex = 5;
            this.label2.Text = "加速(PPS/s)";
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(4, 61);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(59, 12);
            this.label1.TabIndex = 4;
            this.label1.Text = "速度(PPS)";
            // 
            // textBoxAccelerate
            // 
            this.textBoxAccelerate.Location = new System.Drawing.Point(0, 115);
            this.textBoxAccelerate.Name = "textBoxAccelerate";
            this.textBoxAccelerate.Size = new System.Drawing.Size(75, 21);
            this.textBoxAccelerate.TabIndex = 3;
            // 
            // textBoxSpeed
            // 
            this.textBoxSpeed.Location = new System.Drawing.Point(0, 76);
            this.textBoxSpeed.Name = "textBoxSpeed";
            this.textBoxSpeed.Size = new System.Drawing.Size(75, 21);
            this.textBoxSpeed.TabIndex = 2;
            // 
            // radioButtonY
            // 
            this.radioButtonY.AutoSize = true;
            this.radioButtonY.Location = new System.Drawing.Point(6, 42);
            this.radioButtonY.Name = "radioButtonY";
            this.radioButtonY.Size = new System.Drawing.Size(29, 16);
            this.radioButtonY.TabIndex = 1;
            this.radioButtonY.TabStop = true;
            this.radioButtonY.Text = "Y";
            this.radioButtonY.UseVisualStyleBackColor = true;
            // 
            // radioButtonX
            // 
            this.radioButtonX.AutoSize = true;
            this.radioButtonX.Location = new System.Drawing.Point(6, 20);
            this.radioButtonX.Name = "radioButtonX";
            this.radioButtonX.Size = new System.Drawing.Size(29, 16);
            this.radioButtonX.TabIndex = 0;
            this.radioButtonX.TabStop = true;
            this.radioButtonX.Text = "X";
            this.radioButtonX.UseVisualStyleBackColor = true;
            this.radioButtonX.CheckedChanged += new System.EventHandler(this.radioButtonX_CheckedChanged);
            // 
            // buttonX
            // 
            this.buttonX.Location = new System.Drawing.Point(0, 250);
            this.buttonX.Name = "buttonX";
            this.buttonX.Size = new System.Drawing.Size(75, 23);
            this.buttonX.TabIndex = 5;
            this.buttonX.Text = "X开始";
            this.buttonX.UseVisualStyleBackColor = true;
            this.buttonX.Click += new System.EventHandler(this.buttonX_Click);
            // 
            // buttonY
            // 
            this.buttonY.Location = new System.Drawing.Point(0, 279);
            this.buttonY.Name = "buttonY";
            this.buttonY.Size = new System.Drawing.Size(75, 23);
            this.buttonY.TabIndex = 6;
            this.buttonY.Text = "Y开始";
            this.buttonY.UseVisualStyleBackColor = true;
            this.buttonY.Click += new System.EventHandler(this.buttonY_Click);
            // 
            // buttonOptic
            // 
            this.buttonOptic.Location = new System.Drawing.Point(0, 308);
            this.buttonOptic.Name = "buttonOptic";
            this.buttonOptic.Size = new System.Drawing.Size(75, 23);
            this.buttonOptic.TabIndex = 7;
            this.buttonOptic.Text = "光闸开";
            this.buttonOptic.UseVisualStyleBackColor = true;
            this.buttonOptic.Click += new System.EventHandler(this.buttonOptic_Click);
            // 
            // checkBoxReverse
            // 
            this.checkBoxReverse.AutoSize = true;
            this.checkBoxReverse.Location = new System.Drawing.Point(0, 228);
            this.checkBoxReverse.Name = "checkBoxReverse";
            this.checkBoxReverse.Size = new System.Drawing.Size(48, 16);
            this.checkBoxReverse.TabIndex = 8;
            this.checkBoxReverse.Text = "反向";
            this.checkBoxReverse.UseVisualStyleBackColor = true;
            // 
            // buttonManufact
            // 
            this.buttonManufact.Location = new System.Drawing.Point(0, 337);
            this.buttonManufact.Name = "buttonManufact";
            this.buttonManufact.Size = new System.Drawing.Size(75, 23);
            this.buttonManufact.TabIndex = 9;
            this.buttonManufact.Text = "制造开始";
            this.buttonManufact.UseVisualStyleBackColor = true;
            this.buttonManufact.Click += new System.EventHandler(this.buttonManufact_Click);
            // 
            // buttonDisp2D
            // 
            this.buttonDisp2D.Location = new System.Drawing.Point(0, 366);
            this.buttonDisp2D.Name = "buttonDisp2D";
            this.buttonDisp2D.Size = new System.Drawing.Size(75, 23);
            this.buttonDisp2D.TabIndex = 10;
            this.buttonDisp2D.Text = "显示2D视图";
            this.buttonDisp2D.UseVisualStyleBackColor = true;
            this.buttonDisp2D.Click += new System.EventHandler(this.buttonDisp2D_Click);
            // 
            // panel2D
            // 
            this.panel2D.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.panel2D.Location = new System.Drawing.Point(395, 28);
            this.panel2D.Name = "panel2D";
            this.panel2D.Size = new System.Drawing.Size(202, 165);
            this.panel2D.TabIndex = 0;
            // 
            // timerManufact
            // 
            this.timerManufact.Tick += new System.EventHandler(this.timerManufact_Tick);
            // 
            // numericUpDownManufactSpeed
            // 
            this.numericUpDownManufactSpeed.Location = new System.Drawing.Point(0, 407);
            this.numericUpDownManufactSpeed.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownManufactSpeed.Name = "numericUpDownManufactSpeed";
            this.numericUpDownManufactSpeed.Size = new System.Drawing.Size(35, 21);
            this.numericUpDownManufactSpeed.TabIndex = 11;
            this.numericUpDownManufactSpeed.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(-2, 392);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(77, 12);
            this.label3.TabIndex = 12;
            this.label3.Text = "单步制造时间";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(34, 409);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(41, 12);
            this.label4.TabIndex = 13;
            this.label4.Text = "x100ms";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(597, 470);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numericUpDownManufactSpeed);
            this.Controls.Add(this.panel2D);
            this.Controls.Add(this.buttonDisp2D);
            this.Controls.Add(this.buttonManufact);
            this.Controls.Add(this.checkBoxReverse);
            this.Controls.Add(this.buttonOptic);
            this.Controls.Add(this.buttonY);
            this.Controls.Add(this.buttonX);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.statusStrip);
            this.Controls.Add(this.panelDisplay);
            this.Controls.Add(this.buttonConnectPCI);
            this.Controls.Add(this.menuStrip);
            this.MainMenuStrip = this.menuStrip;
            this.Name = "Form1";
            this.Text = "Form1";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.Form1_FormClosing);
            this.menuStrip.ResumeLayout(false);
            this.menuStrip.PerformLayout();
            this.statusStrip.ResumeLayout(false);
            this.statusStrip.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownManufactSpeed)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonConnectPCI;
        private System.Windows.Forms.MenuStrip menuStrip;
        private System.Windows.Forms.ToolStripMenuItem ToolStripMenuItemFile;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItemOpenFile;
        private System.Windows.Forms.Panel panelDisplay;
        private System.Windows.Forms.StatusStrip statusStrip;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button buttonSpeed;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox textBoxAccelerate;
        private System.Windows.Forms.TextBox textBoxSpeed;
        private System.Windows.Forms.RadioButton radioButtonY;
        private System.Windows.Forms.RadioButton radioButtonX;
        private System.Windows.Forms.Button buttonX;
        private System.Windows.Forms.Button buttonY;
        private System.Windows.Forms.Button buttonOptic;
        private System.Windows.Forms.CheckBox checkBoxReverse;
        private System.Windows.Forms.Button buttonManufact;
        private System.Windows.Forms.Button buttonDisp2D;
        private System.Windows.Forms.Panel panel2D;
        private System.Windows.Forms.Timer timerManufact;
        private System.Windows.Forms.NumericUpDown numericUpDownManufactSpeed;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
    }
}


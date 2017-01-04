namespace Sys
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.label5 = new System.Windows.Forms.Label();
            this.textBox_AD0 = new System.Windows.Forms.TextBox();
            this.textBox_AD1 = new System.Windows.Forms.TextBox();
            this.textBox_AD2 = new System.Windows.Forms.TextBox();
            this.textBox_AD3 = new System.Windows.Forms.TextBox();
            this.button_Start = new System.Windows.Forms.Button();
            this.button_stop = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.SuspendLayout();
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(12, 63);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(47, 12);
            this.label2.TabIndex = 2;
            this.label2.Text = "X轴速度";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(247, 65);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(83, 12);
            this.label3.TabIndex = 3;
            this.label3.Text = "X轴逻辑计数器";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(12, 106);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(71, 12);
            this.label4.TabIndex = 4;
            this.label4.Text = "X轴加速度　";
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(249, 106);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(83, 12);
            this.label5.TabIndex = 5;
            this.label5.Text = "Y轴逻辑计数器";
            // 
            // textBox_AD0
            // 
            this.textBox_AD0.Location = new System.Drawing.Point(83, 60);
            this.textBox_AD0.Name = "textBox_AD0";
            this.textBox_AD0.Size = new System.Drawing.Size(121, 21);
            this.textBox_AD0.TabIndex = 6;
            // 
            // textBox_AD1
            // 
            this.textBox_AD1.Location = new System.Drawing.Point(82, 103);
            this.textBox_AD1.Name = "textBox_AD1";
            this.textBox_AD1.Size = new System.Drawing.Size(122, 21);
            this.textBox_AD1.TabIndex = 7;
            // 
            // textBox_AD2
            // 
            this.textBox_AD2.Location = new System.Drawing.Point(333, 60);
            this.textBox_AD2.Name = "textBox_AD2";
            this.textBox_AD2.Size = new System.Drawing.Size(117, 21);
            this.textBox_AD2.TabIndex = 8;
            // 
            // textBox_AD3
            // 
            this.textBox_AD3.Location = new System.Drawing.Point(333, 103);
            this.textBox_AD3.Name = "textBox_AD3";
            this.textBox_AD3.Size = new System.Drawing.Size(117, 21);
            this.textBox_AD3.TabIndex = 9;
            // 
            // button_Start
            // 
            this.button_Start.Location = new System.Drawing.Point(82, 166);
            this.button_Start.Name = "button_Start";
            this.button_Start.Size = new System.Drawing.Size(117, 23);
            this.button_Start.TabIndex = 10;
            this.button_Start.Text = "开始";
            this.button_Start.UseVisualStyleBackColor = true;
            this.button_Start.Click += new System.EventHandler(this.button_Start_Click);
            // 
            // button_stop
            // 
            this.button_stop.Location = new System.Drawing.Point(267, 166);
            this.button_stop.Name = "button_stop";
            this.button_stop.Size = new System.Drawing.Size(117, 23);
            this.button_stop.TabIndex = 11;
            this.button_stop.Text = "停止";
            this.button_stop.UseVisualStyleBackColor = true;
            this.button_stop.Click += new System.EventHandler(this.button_stop_Click);
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(473, 215);
            this.Controls.Add(this.button_stop);
            this.Controls.Add(this.button_Start);
            this.Controls.Add(this.textBox_AD3);
            this.Controls.Add(this.textBox_AD2);
            this.Controls.Add(this.textBox_AD1);
            this.Controls.Add(this.textBox_AD0);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.label2);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "Form1";
            this.Text = "PCI1010";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.TextBox textBox_AD0;
        private System.Windows.Forms.TextBox textBox_AD1;
        private System.Windows.Forms.TextBox textBox_AD2;
        private System.Windows.Forms.TextBox textBox_AD3;
        private System.Windows.Forms.Button button_Start;
        private System.Windows.Forms.Button button_stop;
        private System.Windows.Forms.Timer timer1;
    }
}


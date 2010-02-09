namespace BaseTest.Net
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
            this.IdleTimer = new System.Windows.Forms.Timer(this.components);
            this.Canvas = new System.Windows.Forms.Panel();
            this.Right = new System.Windows.Forms.Button();
            this.Down = new System.Windows.Forms.Button();
            this.Left = new System.Windows.Forms.Button();
            this.Up = new System.Windows.Forms.Button();
            this.Forword = new System.Windows.Forms.Button();
            this.Back = new System.Windows.Forms.Button();
            this.m_ObjectProperty = new System.Windows.Forms.PropertyGrid();
            this.PitchDown = new System.Windows.Forms.Button();
            this.PitchUp = new System.Windows.Forms.Button();
            this.RightControl = new System.Windows.Forms.TabControl();
            this.tabObject = new System.Windows.Forms.TabPage();
            this.tabCamera = new System.Windows.Forms.TabPage();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.restCameraToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.RightControl.SuspendLayout();
            this.tabObject.SuspendLayout();
            this.tabCamera.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.SuspendLayout();
            // 
            // IdleTimer
            // 
            this.IdleTimer.Enabled = true;
            this.IdleTimer.Interval = 1;
            // 
            // Canvas
            // 
            this.Canvas.BackColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.Canvas.Dock = System.Windows.Forms.DockStyle.Fill;
            this.Canvas.Font = new System.Drawing.Font("Microsoft YaHei", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.Canvas.ForeColor = System.Drawing.Color.CornflowerBlue;
            this.Canvas.Location = new System.Drawing.Point(0, 0);
            this.Canvas.Name = "Canvas";
            this.Canvas.Size = new System.Drawing.Size(854, 546);
            this.Canvas.TabIndex = 3;
            this.Canvas.PreviewKeyDown += new System.Windows.Forms.PreviewKeyDownEventHandler(this.Canvas_PreviewKeyDown);
            this.Canvas.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseMove);
            this.Canvas.MouseDown += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseDown);
            this.Canvas.Resize += new System.EventHandler(this.Form1_ResizeEnd);
            this.Canvas.MouseUp += new System.Windows.Forms.MouseEventHandler(this.Canvas_MouseUp);
            // 
            // Right
            // 
            this.Right.Location = new System.Drawing.Point(144, 51);
            this.Right.Name = "Right";
            this.Right.Size = new System.Drawing.Size(47, 29);
            this.Right.TabIndex = 1;
            this.Right.Text = "Right";
            this.Right.UseVisualStyleBackColor = true;
            this.Right.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Right_MouseMove);
            // 
            // Down
            // 
            this.Down.Location = new System.Drawing.Point(145, 128);
            this.Down.Name = "Down";
            this.Down.Size = new System.Drawing.Size(47, 29);
            this.Down.TabIndex = 1;
            this.Down.Text = "Down";
            this.Down.UseVisualStyleBackColor = true;
            this.Down.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Down_MouseMove);
            // 
            // Left
            // 
            this.Left.Location = new System.Drawing.Point(35, 51);
            this.Left.Name = "Left";
            this.Left.Size = new System.Drawing.Size(47, 29);
            this.Left.TabIndex = 1;
            this.Left.Text = "Left";
            this.Left.UseVisualStyleBackColor = true;
            this.Left.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Left_MouseMove);
            // 
            // Up
            // 
            this.Up.Location = new System.Drawing.Point(36, 128);
            this.Up.Name = "Up";
            this.Up.Size = new System.Drawing.Size(47, 29);
            this.Up.TabIndex = 1;
            this.Up.Text = "Up";
            this.Up.UseVisualStyleBackColor = true;
            this.Up.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Up_MouseMove);
            // 
            // Forword
            // 
            this.Forword.Location = new System.Drawing.Point(88, 16);
            this.Forword.Name = "Forword";
            this.Forword.Size = new System.Drawing.Size(47, 29);
            this.Forword.TabIndex = 1;
            this.Forword.Text = "Forword";
            this.Forword.UseVisualStyleBackColor = true;
            this.Forword.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Forword_MouseMove);
            // 
            // Back
            // 
            this.Back.Location = new System.Drawing.Point(88, 51);
            this.Back.Name = "Back";
            this.Back.Size = new System.Drawing.Size(47, 29);
            this.Back.TabIndex = 1;
            this.Back.Text = "Back";
            this.Back.UseVisualStyleBackColor = true;
            this.Back.MouseMove += new System.Windows.Forms.MouseEventHandler(this.Back_MouseMove);
            // 
            // m_ObjectProperty
            // 
            this.m_ObjectProperty.Dock = System.Windows.Forms.DockStyle.Fill;
            this.m_ObjectProperty.Location = new System.Drawing.Point(3, 3);
            this.m_ObjectProperty.Name = "m_ObjectProperty";
            this.m_ObjectProperty.Size = new System.Drawing.Size(202, 514);
            this.m_ObjectProperty.TabIndex = 2;
            // 
            // PitchDown
            // 
            this.PitchDown.Location = new System.Drawing.Point(36, 163);
            this.PitchDown.Name = "PitchDown";
            this.PitchDown.Size = new System.Drawing.Size(77, 30);
            this.PitchDown.TabIndex = 3;
            this.PitchDown.Text = "Pitch-";
            this.PitchDown.UseVisualStyleBackColor = true;
            this.PitchDown.MouseMove += new System.Windows.Forms.MouseEventHandler(this.PitchDown_MouseMove);
            // 
            // PitchUp
            // 
            this.PitchUp.Location = new System.Drawing.Point(119, 163);
            this.PitchUp.Name = "PitchUp";
            this.PitchUp.Size = new System.Drawing.Size(77, 30);
            this.PitchUp.TabIndex = 3;
            this.PitchUp.Text = "Pitch+";
            this.PitchUp.UseVisualStyleBackColor = true;
            this.PitchUp.MouseMove += new System.Windows.Forms.MouseEventHandler(this.PitchUp_MouseMove);
            // 
            // RightControl
            // 
            this.RightControl.Controls.Add(this.tabObject);
            this.RightControl.Controls.Add(this.tabCamera);
            this.RightControl.Dock = System.Windows.Forms.DockStyle.Fill;
            this.RightControl.Location = new System.Drawing.Point(0, 0);
            this.RightControl.Name = "RightControl";
            this.RightControl.SelectedIndex = 0;
            this.RightControl.Size = new System.Drawing.Size(216, 546);
            this.RightControl.TabIndex = 4;
            // 
            // tabObject
            // 
            this.tabObject.Controls.Add(this.m_ObjectProperty);
            this.tabObject.Location = new System.Drawing.Point(4, 22);
            this.tabObject.Name = "tabObject";
            this.tabObject.Padding = new System.Windows.Forms.Padding(3);
            this.tabObject.Size = new System.Drawing.Size(208, 520);
            this.tabObject.TabIndex = 0;
            this.tabObject.Text = "Object";
            this.tabObject.UseVisualStyleBackColor = true;
            // 
            // tabCamera
            // 
            this.tabCamera.Controls.Add(this.Forword);
            this.tabCamera.Controls.Add(this.PitchUp);
            this.tabCamera.Controls.Add(this.Up);
            this.tabCamera.Controls.Add(this.PitchDown);
            this.tabCamera.Controls.Add(this.Left);
            this.tabCamera.Controls.Add(this.Down);
            this.tabCamera.Controls.Add(this.Right);
            this.tabCamera.Controls.Add(this.Back);
            this.tabCamera.Location = new System.Drawing.Point(4, 22);
            this.tabCamera.Name = "tabCamera";
            this.tabCamera.Padding = new System.Windows.Forms.Padding(3);
            this.tabCamera.Size = new System.Drawing.Size(208, 520);
            this.tabCamera.TabIndex = 1;
            this.tabCamera.Text = "Camera";
            this.tabCamera.UseVisualStyleBackColor = true;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(1074, 25);
            this.menuStrip1.TabIndex = 5;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(39, 21);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(96, 22);
            this.exitToolStripMenuItem.Text = "Exit";
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.restCameraToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(42, 21);
            this.editToolStripMenuItem.Text = "Edit";
            // 
            // restCameraToolStripMenuItem
            // 
            this.restCameraToolStripMenuItem.Name = "restCameraToolStripMenuItem";
            this.restCameraToolStripMenuItem.Size = new System.Drawing.Size(146, 22);
            this.restCameraToolStripMenuItem.Text = "RestCamera";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(47, 21);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.aboutToolStripMenuItem.Text = "About";
            this.aboutToolStripMenuItem.Click += new System.EventHandler(this.aboutToolStripMenuItem_Click);
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.Location = new System.Drawing.Point(0, 25);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.Canvas);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.RightControl);
            this.splitContainer1.Size = new System.Drawing.Size(1074, 546);
            this.splitContainer1.SplitterDistance = 854;
            this.splitContainer1.TabIndex = 6;
            // 
            // Form1
            // 
            this.AllowDrop = true;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Window;
            this.ClientSize = new System.Drawing.Size(1074, 571);
            this.Controls.Add(this.splitContainer1);
            this.Controls.Add(this.menuStrip1);
            this.DoubleBuffered = true;
            this.KeyPreview = true;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "Form1";
            this.Text = "eVol3D.NET测试程序";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            this.ResizeEnd += new System.EventHandler(this.Form1_ResizeEnd);
            this.RightControl.ResumeLayout(false);
            this.tabObject.ResumeLayout(false);
            this.tabCamera.ResumeLayout(false);
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            this.splitContainer1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        public System.Windows.Forms.Timer IdleTimer;
        private System.Windows.Forms.Panel Canvas;
        private System.Windows.Forms.Button Right;
        private System.Windows.Forms.Button Down;
        private System.Windows.Forms.Button Left;
        private System.Windows.Forms.Button Up;
        private System.Windows.Forms.Button Forword;
        private System.Windows.Forms.Button Back;
        private System.Windows.Forms.PropertyGrid m_ObjectProperty;
        private System.Windows.Forms.Button PitchDown;
        private System.Windows.Forms.Button PitchUp;
        private System.Windows.Forms.TabControl RightControl;
        private System.Windows.Forms.TabPage tabObject;
        private System.Windows.Forms.TabPage tabCamera;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem restCameraToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.SplitContainer splitContainer1;
    }
}


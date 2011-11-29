namespace NCamGS
{
    partial class MainForm
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
            this.ColourTypeComboBox = new System.Windows.Forms.ComboBox();
            this.paintButton = new System.Windows.Forms.Button();
            this.ResolutionComboBox = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.pictureBox = new System.Windows.Forms.PictureBox();
            this.leftButton = new System.Windows.Forms.Button();
            this.rightButton = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.deleteButton = new System.Windows.Forms.Button();
            this.progressBar = new System.Windows.Forms.ToolStripProgressBar();
            this.statusLabel = new System.Windows.Forms.ToolStripStatusLabel();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.bindingSource1 = new System.Windows.Forms.BindingSource(this.components);
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.mnuOpen = new System.Windows.Forms.ToolStripMenuItem();
            this.saveMnu = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.filePathTextBox = new System.Windows.Forms.TextBox();
            this.filePathButton = new System.Windows.Forms.Button();
            this.label3 = new System.Windows.Forms.Label();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.stopButton = new System.Windows.Forms.Button();
            this.connectButton = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).BeginInit();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // ColourTypeComboBox
            // 
            this.ColourTypeComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ColourTypeComboBox.FormattingEnabled = true;
            this.ColourTypeComboBox.Items.AddRange(new object[] {
            "2-bit Gray Scale(RAW, 2-bit for Y only)",
            "4-bit Gray Scale(RAW,4-bit for Y only)",
            "8-bit Gray Scale(RAW, 8-bit for Y only)",
            "8-bit Colour (RAW,332(RGB))",
            "12-bit Colour(RAW,332(RGB))",
            "16-bit Colour(RAW,565(RGB))",
            "JPEG"});
            this.ColourTypeComboBox.Location = new System.Drawing.Point(138, 27);
            this.ColourTypeComboBox.Name = "ColourTypeComboBox";
            this.ColourTypeComboBox.Size = new System.Drawing.Size(188, 21);
            this.ColourTypeComboBox.TabIndex = 0;
            // 
            // paintButton
            // 
            this.paintButton.Location = new System.Drawing.Point(97, 94);
            this.paintButton.Name = "paintButton";
            this.paintButton.Size = new System.Drawing.Size(75, 23);
            this.paintButton.TabIndex = 0;
            this.paintButton.Text = "&Get Picture";
            this.paintButton.Click += new System.EventHandler(this.paintButton_Click);
            // 
            // ResolutionComboBox
            // 
            this.ResolutionComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.ResolutionComboBox.FormattingEnabled = true;
            this.ResolutionComboBox.Items.AddRange(new object[] {
            "  80 x 64 ",
            "160 x 128 ",
            "320 x 240",
            "640 x 480"});
            this.ResolutionComboBox.Location = new System.Drawing.Point(138, 57);
            this.ResolutionComboBox.Name = "ResolutionComboBox";
            this.ResolutionComboBox.Size = new System.Drawing.Size(188, 21);
            this.ResolutionComboBox.TabIndex = 1;
            this.ResolutionComboBox.SelectedIndexChanged += new System.EventHandler(this.ResolutionComboBox_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 30);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(64, 13);
            this.label1.TabIndex = 3;
            this.label1.Text = "Colour Type";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 57);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(57, 13);
            this.label2.TabIndex = 4;
            this.label2.Text = "Resolution";
            // 
            // pictureBox
            // 
            this.pictureBox.AccessibleRole = System.Windows.Forms.AccessibleRole.PushButton;
            this.pictureBox.Location = new System.Drawing.Point(376, 27);
            this.pictureBox.Name = "pictureBox";
            this.pictureBox.Size = new System.Drawing.Size(440, 330);
            this.pictureBox.TabIndex = 6;
            this.pictureBox.TabStop = false;
            this.pictureBox.Click += new System.EventHandler(this.pictureBox_Click);
            // 
            // leftButton
            // 
            this.leftButton.Location = new System.Drawing.Point(347, 182);
            this.leftButton.Name = "leftButton";
            this.leftButton.Size = new System.Drawing.Size(23, 23);
            this.leftButton.TabIndex = 7;
            this.leftButton.Text = "<";
            this.leftButton.UseVisualStyleBackColor = true;
            this.leftButton.Click += new System.EventHandler(this.leftButton_Click);
            // 
            // rightButton
            // 
            this.rightButton.Location = new System.Drawing.Point(822, 182);
            this.rightButton.Name = "rightButton";
            this.rightButton.Size = new System.Drawing.Size(23, 23);
            this.rightButton.TabIndex = 8;
            this.rightButton.Text = ">";
            this.rightButton.UseVisualStyleBackColor = true;
            this.rightButton.Click += new System.EventHandler(this.rightButton_Click);
            // 
            // button4
            // 
            this.button4.Location = new System.Drawing.Point(16, 94);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(75, 23);
            this.button4.TabIndex = 12;
            this.button4.Text = "&AutoTrigger";
            this.button4.UseVisualStyleBackColor = true;
            // 
            // deleteButton
            // 
            this.deleteButton.Location = new System.Drawing.Point(178, 94);
            this.deleteButton.Name = "deleteButton";
            this.deleteButton.Size = new System.Drawing.Size(75, 23);
            this.deleteButton.TabIndex = 14;
            this.deleteButton.Text = "&Delete";
            this.deleteButton.UseVisualStyleBackColor = true;
            this.deleteButton.Click += new System.EventHandler(this.deleteButton_Click);
            // 
            // progressBar
            // 
            this.progressBar.Name = "progressBar";
            this.progressBar.Size = new System.Drawing.Size(100, 16);
            // 
            // statusLabel
            // 
            this.statusLabel.Name = "statusLabel";
            this.statusLabel.Size = new System.Drawing.Size(103, 17);
            this.statusLabel.Text = "status:No Loading";
            this.statusLabel.TextDirection = System.Windows.Forms.ToolStripTextDirection.Horizontal;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.progressBar,
            this.statusLabel});
            this.statusStrip1.Location = new System.Drawing.Point(0, 366);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(858, 22);
            this.statusStrip1.TabIndex = 11;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "test";
            this.openFileDialog1.InitialDirectory = "\"C:\\Users\\peak\\Pictures\\\"";
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(858, 24);
            this.menuStrip1.TabIndex = 18;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.mnuOpen,
            this.saveMnu,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "File";
            // 
            // mnuOpen
            // 
            this.mnuOpen.Name = "mnuOpen";
            this.mnuOpen.Size = new System.Drawing.Size(114, 22);
            this.mnuOpen.Text = "&Open";
            this.mnuOpen.Click += new System.EventHandler(this.mnuOpen_Click);
            // 
            // saveMnu
            // 
            this.saveMnu.Name = "saveMnu";
            this.saveMnu.Size = new System.Drawing.Size(114, 22);
            this.saveMnu.Text = "&Save As";
            this.saveMnu.Click += new System.EventHandler(this.saveMnu_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(114, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // filePathTextBox
            // 
            this.filePathTextBox.Location = new System.Drawing.Point(12, 185);
            this.filePathTextBox.Name = "filePathTextBox";
            this.filePathTextBox.Size = new System.Drawing.Size(267, 20);
            this.filePathTextBox.TabIndex = 19;
            // 
            // filePathButton
            // 
            this.filePathButton.Location = new System.Drawing.Point(287, 182);
            this.filePathButton.Name = "filePathButton";
            this.filePathButton.Size = new System.Drawing.Size(39, 23);
            this.filePathButton.TabIndex = 20;
            this.filePathButton.Text = "...";
            this.filePathButton.UseVisualStyleBackColor = true;
            this.filePathButton.Click += new System.EventHandler(this.filePathButton_Click);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(12, 169);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(109, 13);
            this.label3.TabIndex = 21;
            this.label3.Text = "Choose your file path:";
            // 
            // stopButton
            // 
            this.stopButton.Location = new System.Drawing.Point(259, 94);
            this.stopButton.Name = "stopButton";
            this.stopButton.Size = new System.Drawing.Size(75, 23);
            this.stopButton.TabIndex = 22;
            this.stopButton.Text = "&Stop";
            this.stopButton.UseVisualStyleBackColor = true;
            this.stopButton.Click += new System.EventHandler(this.stopButton_Click);
            // 
            // connectButton
            // 
            this.connectButton.Location = new System.Drawing.Point(16, 134);
            this.connectButton.Name = "connectButton";
            this.connectButton.Size = new System.Drawing.Size(75, 23);
            this.connectButton.TabIndex = 23;
            this.connectButton.Text = "&Connect";
            this.connectButton.UseVisualStyleBackColor = true;
            this.connectButton.Click += new System.EventHandler(this.connectButton_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(858, 388);
            this.Controls.Add(this.connectButton);
            this.Controls.Add(this.stopButton);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.filePathButton);
            this.Controls.Add(this.filePathTextBox);
            this.Controls.Add(this.deleteButton);
            this.Controls.Add(this.button4);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.rightButton);
            this.Controls.Add(this.leftButton);
            this.Controls.Add(this.pictureBox);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ResolutionComboBox);
            this.Controls.Add(this.paintButton);
            this.Controls.Add(this.ColourTypeComboBox);
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "MainForm";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
            this.Load += new System.EventHandler(this.MainForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.bindingSource1)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox ColourTypeComboBox;
        private System.Windows.Forms.Button paintButton;
        private System.Windows.Forms.ComboBox ResolutionComboBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pictureBox;
        private System.Windows.Forms.Button leftButton;
        private System.Windows.Forms.Button rightButton;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button deleteButton;
        private System.Windows.Forms.ToolStripProgressBar progressBar;
        private System.Windows.Forms.ToolStripStatusLabel statusLabel;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.BindingSource bindingSource1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem mnuOpen;
        private System.Windows.Forms.ToolStripMenuItem saveMnu;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.TextBox filePathTextBox;
        private System.Windows.Forms.Button filePathButton;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
        private System.Windows.Forms.Button stopButton;
        private System.Windows.Forms.Button connectButton;
    }
}
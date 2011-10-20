namespace ImageDisplay
{
    partial class ImageHandler
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
            this.pictureBoxPeak = new System.Windows.Forms.PictureBox();
            this.ConvertToByteButton = new System.Windows.Forms.Button();
            this.textBoxImageByteConversion = new System.Windows.Forms.TextBox();
            this.pictureBoxConverted = new System.Windows.Forms.PictureBox();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPeak)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxConverted)).BeginInit();
            this.SuspendLayout();
            // 
            // pictureBoxPeak
            // 
            this.pictureBoxPeak.AccessibleName = "pictureBoxPeak";
            this.pictureBoxPeak.Location = new System.Drawing.Point(12, 12);
            this.pictureBoxPeak.Name = "pictureBoxPeak";
            this.pictureBoxPeak.Size = new System.Drawing.Size(292, 238);
            this.pictureBoxPeak.TabIndex = 0;
            this.pictureBoxPeak.TabStop = false;
            // 
            // ConvertToByteButton
            // 
            this.ConvertToByteButton.Location = new System.Drawing.Point(208, 256);
            this.ConvertToByteButton.Name = "ConvertToByteButton";
            this.ConvertToByteButton.Size = new System.Drawing.Size(96, 23);
            this.ConvertToByteButton.TabIndex = 1;
            this.ConvertToByteButton.Text = "convert to Byte";
            this.ConvertToByteButton.UseVisualStyleBackColor = true;
            this.ConvertToByteButton.Click += new System.EventHandler(this.ConvertToByteButton_Click);
            // 
            // textBoxImageByteConversion
            // 
            this.textBoxImageByteConversion.Location = new System.Drawing.Point(311, 13);
            this.textBoxImageByteConversion.Multiline = true;
            this.textBoxImageByteConversion.Name = "textBoxImageByteConversion";
            this.textBoxImageByteConversion.Size = new System.Drawing.Size(492, 100);
            this.textBoxImageByteConversion.TabIndex = 2;
            // 
            // pictureBoxConverted
            // 
            this.pictureBoxConverted.Location = new System.Drawing.Point(477, 175);
            this.pictureBoxConverted.Name = "pictureBoxConverted";
            this.pictureBoxConverted.Size = new System.Drawing.Size(285, 235);
            this.pictureBoxConverted.TabIndex = 3;
            this.pictureBoxConverted.TabStop = false;
            // 
            // ImageHandler
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(827, 448);
            this.Controls.Add(this.pictureBoxConverted);
            this.Controls.Add(this.textBoxImageByteConversion);
            this.Controls.Add(this.ConvertToByteButton);
            this.Controls.Add(this.pictureBoxPeak);
            this.Name = "ImageHandler";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxPeak)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxConverted)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBoxPeak;
        private System.Windows.Forms.Button ConvertToByteButton;
        private System.Windows.Forms.TextBox textBoxImageByteConversion;
        private System.Windows.Forms.PictureBox pictureBoxConverted;
    }
}


/*******************************************************************************
* Copyright 2012-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

namespace Bootloader_v1_30
{
    partial class CyGeneralPage
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

        #region Component Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(CyGeneralPage));
            this.errProvider = new System.Windows.Forms.ErrorProvider(this.components);
            this.groupBoxOptions = new System.Windows.Forms.GroupBox();
            this.checkBoxBrAppValidation = new System.Windows.Forms.CheckBox();
            this.comboBoxIOComponent = new System.Windows.Forms.ComboBox();
            this.labelIOComponent = new System.Windows.Forms.Label();
            this.checkBoxMultiApp = new System.Windows.Forms.CheckBox();
            this.labelZero = new System.Windows.Forms.Label();
            this.checkBoxBeAppValidation = new System.Windows.Forms.CheckBox();
            this.numUpDownWaitTime = new System.Windows.Forms.NumericUpDown();
            this.labelPktChecksumType = new System.Windows.Forms.Label();
            this.comboBoxPktChecksumType = new System.Windows.Forms.ComboBox();
            this.checkBoxWaitTime = new System.Windows.Forms.CheckBox();
            this.textBoxVersion = new System.Windows.Forms.TextBox();
            this.labelVersion = new System.Windows.Forms.Label();
            this.labelWaitTime = new System.Windows.Forms.Label();
            this.groupBoxCommands = new System.Windows.Forms.GroupBox();
            this.checkBoxCommMetadata = new System.Windows.Forms.CheckBox();
            this.checkBoxCommStatus = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSync = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSend = new System.Windows.Forms.CheckBox();
            this.checkBoxCommVerify = new System.Windows.Forms.CheckBox();
            this.checkBoxCommErase = new System.Windows.Forms.CheckBox();
            this.checkBoxCommSize = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.errProvider)).BeginInit();
            this.groupBoxOptions.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownWaitTime)).BeginInit();
            this.groupBoxCommands.SuspendLayout();
            this.SuspendLayout();
            // 
            // errProvider
            // 
            this.errProvider.BlinkStyle = System.Windows.Forms.ErrorBlinkStyle.NeverBlink;
            this.errProvider.ContainerControl = this;
            // 
            // groupBoxOptions
            // 
            this.groupBoxOptions.Controls.Add(this.checkBoxBrAppValidation);
            this.groupBoxOptions.Controls.Add(this.comboBoxIOComponent);
            this.groupBoxOptions.Controls.Add(this.labelIOComponent);
            this.groupBoxOptions.Controls.Add(this.checkBoxMultiApp);
            this.groupBoxOptions.Controls.Add(this.labelZero);
            this.groupBoxOptions.Controls.Add(this.checkBoxBeAppValidation);
            this.groupBoxOptions.Controls.Add(this.numUpDownWaitTime);
            this.groupBoxOptions.Controls.Add(this.labelPktChecksumType);
            this.groupBoxOptions.Controls.Add(this.comboBoxPktChecksumType);
            this.groupBoxOptions.Controls.Add(this.checkBoxWaitTime);
            this.groupBoxOptions.Controls.Add(this.textBoxVersion);
            this.groupBoxOptions.Controls.Add(this.labelVersion);
            this.groupBoxOptions.Controls.Add(this.labelWaitTime);
            resources.ApplyResources(this.groupBoxOptions, "groupBoxOptions");
            this.groupBoxOptions.Name = "groupBoxOptions";
            this.groupBoxOptions.TabStop = false;
            // 
            // checkBoxBrAppValidation
            // 
            resources.ApplyResources(this.checkBoxBrAppValidation, "checkBoxBrAppValidation");
            this.checkBoxBrAppValidation.Name = "checkBoxBrAppValidation";
            this.checkBoxBrAppValidation.UseVisualStyleBackColor = true;
            this.checkBoxBrAppValidation.CheckedChanged += new System.EventHandler(this.checkBoxOptions_CheckedChanged);
            // 
            // comboBoxIOComponent
            // 
            this.comboBoxIOComponent.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxIOComponent.FormattingEnabled = true;
            resources.ApplyResources(this.comboBoxIOComponent, "comboBoxIOComponent");
            this.comboBoxIOComponent.Name = "comboBoxIOComponent";
            this.comboBoxIOComponent.SelectedIndexChanged += new System.EventHandler(this.comboBoxIOComponent_SelectedIndexChanged);
            // 
            // labelIOComponent
            // 
            resources.ApplyResources(this.labelIOComponent, "labelIOComponent");
            this.labelIOComponent.Name = "labelIOComponent";
            // 
            // checkBoxMultiApp
            // 
            resources.ApplyResources(this.checkBoxMultiApp, "checkBoxMultiApp");
            this.checkBoxMultiApp.Checked = true;
            this.checkBoxMultiApp.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxMultiApp.Name = "checkBoxMultiApp";
            this.checkBoxMultiApp.UseVisualStyleBackColor = true;
            this.checkBoxMultiApp.CheckedChanged += new System.EventHandler(this.checkBoxMultiApp_CheckedChanged);
            // 
            // labelZero
            // 
            resources.ApplyResources(this.labelZero, "labelZero");
            this.labelZero.Name = "labelZero";
            // 
            // checkBoxBeAppValidation
            // 
            resources.ApplyResources(this.checkBoxBeAppValidation, "checkBoxBeAppValidation");
            this.checkBoxBeAppValidation.Name = "checkBoxBeAppValidation";
            this.checkBoxBeAppValidation.UseVisualStyleBackColor = true;
            this.checkBoxBeAppValidation.CheckedChanged += new System.EventHandler(this.checkBoxOptions_CheckedChanged);
            // 
            // numUpDownWaitTime
            // 
            this.numUpDownWaitTime.Increment = new decimal(new int[] {
            100,
            0,
            0,
            0});
            resources.ApplyResources(this.numUpDownWaitTime, "numUpDownWaitTime");
            this.numUpDownWaitTime.Maximum = new decimal(new int[] {
            25500,
            0,
            0,
            0});
            this.numUpDownWaitTime.Name = "numUpDownWaitTime";
            this.numUpDownWaitTime.Value = new decimal(new int[] {
            10,
            0,
            0,
            0});
            this.numUpDownWaitTime.ValueChanged += new System.EventHandler(this.numUpDownWaitTime_ValueChanged);
            // 
            // labelPktChecksumType
            // 
            resources.ApplyResources(this.labelPktChecksumType, "labelPktChecksumType");
            this.labelPktChecksumType.Name = "labelPktChecksumType";
            // 
            // comboBoxPktChecksumType
            // 
            this.comboBoxPktChecksumType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxPktChecksumType.FormattingEnabled = true;
            this.comboBoxPktChecksumType.Items.AddRange(new object[] {
            resources.GetString("comboBoxPktChecksumType.Items"),
            resources.GetString("comboBoxPktChecksumType.Items1")});
            resources.ApplyResources(this.comboBoxPktChecksumType, "comboBoxPktChecksumType");
            this.comboBoxPktChecksumType.Name = "comboBoxPktChecksumType";
            this.comboBoxPktChecksumType.SelectedIndexChanged += new System.EventHandler(this.comboBoxPktChecksumType_SelectedIndexChanged);
            // 
            // checkBoxWaitTime
            // 
            resources.ApplyResources(this.checkBoxWaitTime, "checkBoxWaitTime");
            this.checkBoxWaitTime.Checked = true;
            this.checkBoxWaitTime.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBoxWaitTime.Name = "checkBoxWaitTime";
            this.checkBoxWaitTime.UseVisualStyleBackColor = true;
            this.checkBoxWaitTime.CheckedChanged += new System.EventHandler(this.checkBoxWaitTime_CheckedChanged);
            // 
            // textBoxVersion
            // 
            resources.ApplyResources(this.textBoxVersion, "textBoxVersion");
            this.textBoxVersion.Name = "textBoxVersion";
            this.textBoxVersion.TextChanged += new System.EventHandler(this.textBoxVersion_TextChanged);
            // 
            // labelVersion
            // 
            resources.ApplyResources(this.labelVersion, "labelVersion");
            this.labelVersion.Name = "labelVersion";
            // 
            // labelWaitTime
            // 
            resources.ApplyResources(this.labelWaitTime, "labelWaitTime");
            this.labelWaitTime.Name = "labelWaitTime";
            // 
            // groupBoxCommands
            // 
            this.groupBoxCommands.Controls.Add(this.checkBoxCommMetadata);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommStatus);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSync);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSend);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommVerify);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommErase);
            this.groupBoxCommands.Controls.Add(this.checkBoxCommSize);
            resources.ApplyResources(this.groupBoxCommands, "groupBoxCommands");
            this.groupBoxCommands.Name = "groupBoxCommands";
            this.groupBoxCommands.TabStop = false;
            // 
            // checkBoxCommMetadata
            // 
            resources.ApplyResources(this.checkBoxCommMetadata, "checkBoxCommMetadata");
            this.checkBoxCommMetadata.Name = "checkBoxCommMetadata";
            this.checkBoxCommMetadata.UseVisualStyleBackColor = true;
            this.checkBoxCommMetadata.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommStatus
            // 
            resources.ApplyResources(this.checkBoxCommStatus, "checkBoxCommStatus");
            this.checkBoxCommStatus.Name = "checkBoxCommStatus";
            this.checkBoxCommStatus.UseVisualStyleBackColor = true;
            this.checkBoxCommStatus.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSync
            // 
            resources.ApplyResources(this.checkBoxCommSync, "checkBoxCommSync");
            this.checkBoxCommSync.Name = "checkBoxCommSync";
            this.checkBoxCommSync.UseVisualStyleBackColor = true;
            this.checkBoxCommSync.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSend
            // 
            resources.ApplyResources(this.checkBoxCommSend, "checkBoxCommSend");
            this.checkBoxCommSend.Name = "checkBoxCommSend";
            this.checkBoxCommSend.UseVisualStyleBackColor = true;
            this.checkBoxCommSend.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommVerify
            // 
            resources.ApplyResources(this.checkBoxCommVerify, "checkBoxCommVerify");
            this.checkBoxCommVerify.Name = "checkBoxCommVerify";
            this.checkBoxCommVerify.UseVisualStyleBackColor = true;
            this.checkBoxCommVerify.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommErase
            // 
            resources.ApplyResources(this.checkBoxCommErase, "checkBoxCommErase");
            this.checkBoxCommErase.Name = "checkBoxCommErase";
            this.checkBoxCommErase.UseVisualStyleBackColor = true;
            this.checkBoxCommErase.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // checkBoxCommSize
            // 
            resources.ApplyResources(this.checkBoxCommSize, "checkBoxCommSize");
            this.checkBoxCommSize.Name = "checkBoxCommSize";
            this.checkBoxCommSize.UseVisualStyleBackColor = true;
            this.checkBoxCommSize.CheckedChanged += new System.EventHandler(this.checkBoxComm_CheckedChanged);
            // 
            // CyGeneralPage
            // 
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.Controls.Add(this.groupBoxCommands);
            this.Controls.Add(this.groupBoxOptions);
            this.Name = "CyGeneralPage";
            ((System.ComponentModel.ISupportInitialize)(this.errProvider)).EndInit();
            this.groupBoxOptions.ResumeLayout(false);
            this.groupBoxOptions.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numUpDownWaitTime)).EndInit();
            this.groupBoxCommands.ResumeLayout(false);
            this.groupBoxCommands.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ErrorProvider errProvider;
        private System.Windows.Forms.GroupBox groupBoxCommands;
        private System.Windows.Forms.GroupBox groupBoxOptions;
        private System.Windows.Forms.TextBox textBoxVersion;
        private System.Windows.Forms.Label labelVersion;
        private System.Windows.Forms.Label labelWaitTime;
        private System.Windows.Forms.CheckBox checkBoxBeAppValidation;
        private System.Windows.Forms.CheckBox checkBoxCommStatus;
        private System.Windows.Forms.CheckBox checkBoxCommSync;
        private System.Windows.Forms.CheckBox checkBoxCommSend;
        private System.Windows.Forms.CheckBox checkBoxCommVerify;
        private System.Windows.Forms.CheckBox checkBoxCommErase;
        private System.Windows.Forms.CheckBox checkBoxCommSize;
        private System.Windows.Forms.Label labelPktChecksumType;
        private System.Windows.Forms.ComboBox comboBoxPktChecksumType;
        private System.Windows.Forms.CheckBox checkBoxWaitTime;
        private System.Windows.Forms.NumericUpDown numUpDownWaitTime;
        private System.Windows.Forms.Label labelZero;
        private System.Windows.Forms.ComboBox comboBoxIOComponent;
        private System.Windows.Forms.Label labelIOComponent;
        private System.Windows.Forms.CheckBox checkBoxMultiApp;
        private System.Windows.Forms.CheckBox checkBoxBrAppValidation;
        private System.Windows.Forms.CheckBox checkBoxCommMetadata;
    }
}

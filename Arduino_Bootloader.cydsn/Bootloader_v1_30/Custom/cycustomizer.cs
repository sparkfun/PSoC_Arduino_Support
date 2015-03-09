/*******************************************************************************
* Copyright 2012-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using CyDesigner.Extensions.Gde;
using CyDesigner.Extensions.Common;
using Bootloader_v1_30.Properties;
using CyDesigner.Toolkit.TerminalControl_v1;
using CyTerminalControl_v1 = CyDesigner.Toolkit.TerminalControl_v1.CyTerminalControl_v1;

namespace Bootloader_v1_30
{
    [CyCompDevCustomizer]
    sealed public class CyCustomizer : ICyParamEditHook_v1, ICyBootloaderProvider_v1, ICyDRCProvider_v1,
                                ICySystemBuilderComp_v1
    {
        #region ICyParamEditHook_v1 Members
        DialogResult ICyParamEditHook_v1.EditParams(ICyInstEdit_v1 edit, ICyTerminalQuery_v1 termQuery,
                                                    ICyExpressMgr_v1 mgr)
        {
            const string PAPAM_TAB_NAME_BUILTIN = "Built-in";
            const string PAPAM_TAB_NAME_INTERCONNECT = "Interconnect";
            CyParameters prms = new CyParameters(edit);
            CyGeneralPage generalPage = new CyGeneralPage(prms);
            ICyTabbedParamEditor editor = edit.CreateTabbedParamEditor();
            CyTerminalControl_v1 interconnectTab = null;

            CyParamExprDelegate exprDelegate = delegate(ICyParamEditor custEditor, CyCompDevParam param)
            {
                prms.m_globalEditMode = false;
                if (param.TabName == generalPage.TabName)
                    generalPage.InitFields();
                prms.m_globalEditMode = true;
            };            

            editor.AddCustomPage(Properties.Resources.PageTitleGeneral, generalPage, exprDelegate, generalPage.TabName);
            
            // System Builder support
            if (prms.InstQuery.DeviceQuery.IsPSoC4 && edit.IsInSystemBuilder)
            {
                interconnectTab = new CyTerminalControl_v1(edit);
                editor.AddCustomPage(Resources.InterconnectTabDisplayName, interconnectTab, exprDelegate,
                    PAPAM_TAB_NAME_INTERCONNECT);
            }
            editor.AddDefaultPage(Properties.Resources.PageTitleBuiltIn, PAPAM_TAB_NAME_BUILTIN);

            prms.m_globalEditMode = true;

            return editor.ShowDialog();
        }

        bool ICyParamEditHook_v1.EditParamsOnDrop
        {
            get { return false; }
        }

        CyCompDevParamEditorMode ICyParamEditHook_v1.GetEditorMode()
        {
            return CyCompDevParamEditorMode.COMPLETE;
        }
        #endregion

        #region ICyBootloaderProvider_v1 Members
        public CyCustErr GetBootloaderData(ICyBootloaderProviderArgs_v1 args, out CyBootloaderData_v1 data)
        {
            CyParameters prms = new CyParameters(args.InstQuery);

            CyBootloaderChecksumType_v1 checksum = CyBootloaderChecksumType_v1.BasicChecksum;
            if (prms.PacketChecksumType == true)
            {
                checksum = CyBootloaderChecksumType_v1.CRC16;
            }

            data = new CyBootloaderData_v1(prms.Version, checksum, prms.IOComponent);

            return CyCustErr.OK;
        }
        #endregion

        #region ICyDRCProvider_v1 Members
        public IEnumerable<CyDRCInfo_v1> GetDRCs(ICyDRCProviderArgs_v1 args)
        {
            CyParameters parameters = new CyParameters(args.InstQueryV1);

            if (parameters.CompareAppType() == false)
            {
                yield return new CyDRCInfo_v1(CyDRCInfo_v1.CyDRCType_v1.Error,
                                              Properties.Resources.AppTypeCompareError);
            }

        }
        #endregion      

        #region ICySystemBuilderComp_v1 members
        public CyCustErr GetTitleText(ICySystemBuilderCompArgs_v1 args, out string title)
        {
            title = Properties.Resources.SBComponentTitle;
            return CyCustErr.OK;
        }

        /// <summary>
        /// Converts decimal UInt16 value to hexadecimal string
        /// </summary>
        /// <param name="value">Value to convert</param>
        /// <returns>Hexadecimal string of the following format: 0x0000</returns>
        public static string ConvertDecToHex(UInt16 value)
        {
            return String.Format("0x{0}", value.ToString("X4"));
        }

        public CyCustErr GetBodyText(ICySystemBuilderCompArgs_v1 args, out string bodyText)
        {
            string appVersion = "";
            System.UInt16 iappVersion;

            CyCompDevParam applicationVersion = args.InstQuery.GetCommittedParam(CyParameters.PARAM_VERSION);

            if (applicationVersion.ErrorCount == 0)
            {
                applicationVersion.TryGetValueAs<System.UInt16>(out iappVersion);
                appVersion = ConvertDecToHex(iappVersion);
            }

            bodyText = String.Format(Resources.SBComponentBodyText, appVersion);

            return CyCustErr.OK;
        }
        #endregion
    }
}

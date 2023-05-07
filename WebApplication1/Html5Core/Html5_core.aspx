<%@ Page Title="" Language="C#" MasterPageFile="~/MasterPages/WasmPage.Master" AutoEventWireup="true" CodeBehind="Html5_core.aspx.cs" Inherits="WebApplication1.OpenGlProjV2.WindowsEvents.html5_core" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
    <style>
        body, html {
            margin: 0;
            padding: 0;
            height: 100%;
            width: 100%;
        }
        #canvas {
            position: absolute;
            width: 100%;
            height: 100%;
        }
    </style>
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <canvas id="canvas" ></canvas>
    <script async type="text/javascript" src="html5_core.js"></script>
</asp:Content>

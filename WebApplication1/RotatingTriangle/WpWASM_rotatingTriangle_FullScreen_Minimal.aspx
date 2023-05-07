<%@ Page Title="Open GL Rotating Triangle" Language="C#" MasterPageFile="~/MasterPages/WasmPage.Master" AutoEventWireup="true" CodeBehind="WpWASM_rotatingTriangle_FullScreen_Minimal.aspx.cs" Inherits="WebApplication1.OpenGlProjV2.Base.WpWASM_rotatingTriangle_FullScreen_Minimal" %>
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
    <script async type="text/javascript" src="WpWASM_rotatingTriangle.js"></script>
</asp:Content>

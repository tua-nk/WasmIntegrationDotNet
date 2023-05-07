<%@ Page Title="Wasm Bubble Demo - Resizeable Canvas" Language="C#" MasterPageFile="~/MasterPages/WasmPage.Master" AutoEventWireup="true" CodeBehind="ResizeableCanvas.aspx.cs" Inherits="WebApplication1.WasmPages.Resizeablecanvas" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
        <style media="screen">
        body, canvas {
            width: 100%;
            height: 100%;
            font-family: sans-serif;
            font-size: medium;
            margin: 0;
        }
    </style>
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">

   <canvas id="canvas" width="1000" height="1000"></canvas>
    <script src="canvas.js"></script>
    <script>

      // Create canvas context
      const canvas = document.getElementById('canvas')
      const context = canvas.getContext('2d')

      var startTime, endTime;

      var milliSecs;
      function startTimer() {
        startTime = performance.now();
      };

      function endTimer() {
        endTime = performance.now();
        var timeDiff = endTime - startTime; //in ms
        // strip the ms
        // timeDiff /= 1000;

        // get seconds
        milliSecs= Math.round(timeDiff);
        milliSecs = timeDiff;
        //console.log(milliSecs + " milliSecs");
      }

      funcParam = 1;
      funcResult = 0;
      numTests = 0;
      totalTests = 10000;
      totalTime =0.0;
      // Render circles on canvas
      const render = ( dataLength, circleStructSize ) => {


        // Get circle data from C - Typed Array
        let circles = new Int32Array( Module.HEAP32.buffer, _getCircles( canvas.width, canvas.height ), dataLength )

        // Clear existing canvas to re-render
        context.clearRect( 0, 0, canvas.width, canvas.height )

        startTimer();
        // funcResult =  _doWork(funcParam);
        endTimer();
        console.log("Param = " + funcParam + ", Result = " + funcResult + ", ExecTime (ms) =", milliSecs);
        totalTime+= milliSecs;
        numTests++;
        // funcParam++;

        if (numTests == totalTests)
        {
          console.log("Average time = " + totalTime/totalTests);
        }


        // Loop data and get circle chunks
        for( let i = 0; i < circles.length; i+=circleStructSize ) {

          // Get circle data chunk
          let circle = circles.slice( i, i+circleStructSize )

          // Draw circle
          context.beginPath()
          context.arc(circle[0], circle[1], circle[2], 0, 2*Math.PI, false)
          context.fillStyle = `rgba(${circle[3]},${circle[4]},${circle[5]},0.75)`
          context.fill()
        }

        // Request next animation frame and re-render with updated circles
        window.requestAnimationFrame( () => {
          render( dataLength, circleStructSize )
        })
      }

    </script>

</asp:Content>

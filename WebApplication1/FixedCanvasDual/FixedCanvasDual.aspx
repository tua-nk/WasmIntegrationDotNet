<%@ Page Title="Wasm Bubble Demo - Fixed Size Dual Canvases" Language="C#" MasterPageFile="~/MasterPages/WasmPage.Master" AutoEventWireup="true" CodeBehind="FixedCanvasDual.aspx.cs" Inherits="WebApplication1.WasmPages.FixedCanvasDual" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
        <style media="screen">
        body {
            width: 100%;
            height: 100%;
            font-family: sans-serif;
            font-size: medium;
            margin: 0;
        }
        #canvas1 {
            /* fixed size and position for the canvas*/
            position: absolute;
            left: 50px;
            top: 150px;
            width: 400px;
            height: 300px;
            display: block;
            margin: 0;
            border-style:double;
            border-color:blue;
        }

        #canvas2 {
            /* fixed size and position for the canvas*/
            position: absolute;
            left: 250px;
            top: 250px;
            width: 400px;
            height: 300px;
            display: block;
            margin: 0;
            border-style: double;
            border-color: coral;
        }
    </style>
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
        <canvas id="canvas1" width="2000" height="1500"></canvas>
      <canvas id="canvas2" width="2000" height="1500"></canvas>
      <script src="canvas.js"></script>
      <script>

          // Create canvas contexts
          const canvas1 = document.getElementById('canvas1')
          const context1 = canvas1.getContext('2d')

          const canvas2 = document.getElementById('canvas2')
          const context2 = canvas2.getContext('2d')

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
              milliSecs = Math.round(timeDiff);
              milliSecs = timeDiff;
              //console.log(milliSecs + " milliSecs");
          }

          funcParam = 1;
          funcResult = 0;
          numTests = 0;
          totalTests = 10000;
          totalTime = 0.0;
          // Render circles on canvas
          const render = (dataLength, circleStructSize) => {


              // Get circle data from C - Typed Array
              let circles = new Int32Array(Module.HEAP32.buffer, _getCircles(canvas1.width, canvas1.height), dataLength)

              let circles2 = new Int32Array(Module.HEAP32.buffer, _getCircles(canvas2.width, canvas2.height), dataLength)


              // Clear existing canvas to re-render
              context1.clearRect(0, 0, canvas1.width, canvas1.height)
              context2.clearRect(0, 0, canvas2.width, canvas2.height)

              startTimer();
              // funcResult =  _doWork(funcParam);
              endTimer();
              console.log("Param = " + funcParam + ", Result = " + funcResult + ", ExecTime (ms) =", milliSecs);
              totalTime += milliSecs;
              numTests++;
              // funcParam++;

              if (numTests == totalTests) {
                  console.log("Average time = " + totalTime / totalTests);
              }


              // Loop data and get circle chunks
              for (let i = 0; i < circles.length; i += circleStructSize) {

                  // Get circle data chunk
                  let circle = circles.slice(i, i + circleStructSize)
                  let circle2 = circles2.slice(i, i + circleStructSize)

                  // Draw circle
                  context1.beginPath()
                  context1.arc(circle[0], circle[1], circle[2], 0, 2 * Math.PI, false)
                  context1.fillStyle = `rgba(${circle[3]},${circle[4]},${circle[5]},0.75)`
                  context1.fill()

                  // Draw circle
                  context2.beginPath()
                  context2.arc(circle2[0], circle2[1], circle2[2], 0, 2 * Math.PI, false)
                  context2.fillStyle = `rgba(${circle2[3]},${circle2[4]},${circle2[5]},0.75)`
                  context2.fill()

              }




              // Request next animation frame and re-render with updated circles
              window.requestAnimationFrame(() => {
                  render(dataLength, circleStructSize)
              })
          }

      </script>

</asp:Content>

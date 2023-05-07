<%@ Page Title="Wasm Loader" Language="C#" MasterPageFile="~/MasterPages/WasmPageAutoLoad.Master" AutoEventWireup="true" CodeBehind="WasmLoaderV6.aspx.cs" Inherits="WebApplication1.WasmLoader.WasmLoaderV6" %>
<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
            <style>
            html, body{
                margin : 0;
                padding : 0;
                height: 100%;
            }

            /*  // For filling the whole screen
            #canvas{
                width: 100%;
                height: 100%;
                display: block;
            }
                */
            /*  Below for fixed size */
            #canvas{
                position: absolute;
                left: 350px;   /* replace with the desired x-coordinate in pixels */
                top: 350px;   /* replace with the desired y-coordinate in pixels */
                width: 400px;  /* replace with the desired width in pixels */
                height: 300px;  /* replace with the desired height in pixels */
                display: block;
            }

    /* This is the style for the outer part of the progress bar */
    progress {
        width: 100%;
        height: 20px;
        background-color: #f3f3f3;
        border: none; /* Remove the default border */
        border-radius: 10px; /* Round corners */
    }

    /* This is the style for the inner part of the progress bar (the bar itself) */
    progress::-webkit-progress-bar {
        background-color: #f3f3f3;
        border-radius: 10px;
    }

    progress::-webkit-progress-value {
        background-color: #4CAF50; /* Green color */
        border-radius: 10px;
    }

    /* The same for Mozilla */
    progress::-moz-progress-bar {
        background-color: #4CAF50;
    }
        </style>
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="ContentPlaceHolder1" runat="server">
    <div>
        <h1>Welcome to the WebAssembly Page</h1>
        <p id="status">Click the button below to launch the WebAssembly program.</p>
        <button style="display:none;" id="loadButton" type = "button" onclick = "retryLoading()">Retry Loading</button>
        <progress id="progress" value="0" max="100"></progress>
    </div>

    <canvas id="canvas" ></canvas>

        <script>
            var fetchTimeoutID = 0;
            var startTime = performance.now();
            function updateProgressBar(percentage) {
                console.log("updateProgressBar percentage: " + percentage);
                document.getElementById('progress').value = percentage;
            }
            var jsFile = "WpWASM_rotatingTriangle_huge.js";  // replace with the name of your WebAssembly program's .js file
            var wasmFileBase = "WpWASM_rotatingTriangle_huge.wasm";  // replace with the name of your WebAssembly program's .js file

            var Module = {
                instantiateWasm: function (imports, successCallback) {
                    var wasmFile = wasmFileBase + '?' + new Date().getTime();
                    var controller = new AbortController(); // Create a new AbortController
                    var signal = controller.signal; // Get the associated AbortSignal

                    function startFetchTimeout() {
                        clearTimeout(fetchTimeoutID);  // clear the existing timeout
                        fetchTimeoutID = setTimeout(() => {  // set a new timeout
                            controller.abort();
                            handleLoadError("Network error: Timeout when fetching the WebAssembly file. Please try again.");
                        }, 10000); // 10 seconds timeout
                    }

                    startFetchTimeout();

                    fetch(wasmFile, { signal }) // Pass the signal to the fetch call
                        .then(response => {
                            const totalLength = Number(response.headers.get('Content-Length'));
                            if (!totalLength) {
                                throw new Error('Content-Length response header is missing');
                            }
                            else {
                                console.log("WebAssembly program content length: " + totalLength);
                            }
                            let loadedLength = 0;
                            const reader = response.body.getReader();
                            const stream = new ReadableStream({
                                start(controller) {
                                    read();
                                    function read() {
                                        reader.read().then(({ done, value }) => {
                                            if (done) {
                                                controller.close();
                                                return;
                                            }
                                            loadedLength += value.length;
                                            updateProgressBar(loadedLength / totalLength * 100);
                                            controller.enqueue(value);
                                            startFetchTimeout(); // reset the timeout
                                            read();
                                        }).catch(error => {
                                            console.error('Error while reading from the stream: ', error);
                                            handleLoadError("Failed to load the WebAssembly file due to a network issue. Please try again. Error: " + error.message);
                                            controller.abort(); // Abort the fetch request
                                        });
                                    }
                                }
                            }, {
                                cancel(reason) {
                                    console.error('ReadableStream was cancelled: ', reason);
                                }
                            });
                            return new Response(stream, { headers: response.headers });
                        })
                        .then(response =>
                            WebAssembly.instantiateStreaming(response, imports)
                                .then(results => {
                                    successCallback(results.instance);
                                })
                                .catch(error => {
                                    handleLoadError("Failed to instantiate the WebAssembly file. Please try again. Error: " + error.message);
                                })
                        )
                        .catch(error => {
                            handleLoadError("Failed to fetch the WebAssembly file. Please try again. Error: " + error.message);
                        });
                    return {}; // indicates that instantiation is async and successCallback will be called later
                },
                onRuntimeInitialized: function () {
                    clearTimeout(fetchTimeoutID);
                    document.getElementById('status').textContent = "WebAssembly program has loaded!";
                    var loadTime = performance.now() - startTime;
                    console.log("WebAssembly program loaded in " + loadTime + " ms");
                }
            };


            function launchWebAssemblyProgram() {
                document.getElementById('status').textContent = "WebAssembly program is loading...";
                startTime = performance.now(); // reset value of startTime

                // Chat GPT4 solution, which does work, since it realizes that the .wasm is loaded by the compiler-generated js file. So just load the js.
                var scriptElement = document.createElement("script");
                scriptElement.src = jsFile;
                // Error handling added:
                scriptElement.onerror = function () {
                    document.getElementById('status').textContent = "Error: Failed to load the WebAssembly program.";
                    document.getElementById('loadButton').style.display = "inline-block"; // or "block"
                    document.getElementById('loadButton').textContent = "Retry Loading";
                };
                document.head.appendChild(scriptElement);
            }

            function retryLoading() {

                window.location.reload();
                /*
                // Remove the old script element
                var oldScriptElement = document.querySelector(`script[src="${jsFile}"]`);
                document.head.removeChild(oldScriptElement);
                // Hide the retry button
                document.getElementById('loadButton').style.display = "none"; 
                // Try loading the script again
                launchWebAssemblyProgram();
                */
            }

            function handleLoadError(message) {
                document.getElementById('status').textContent = message;
                document.getElementById('loadButton').style.display = "inline-block";
                document.getElementById('loadButton').textContent = "Retry Loading";

                clearTimeout(fetchTimeoutID); // clear timeout

                // Remove the run dependency added by Emscripten runtime
                Module.removeRunDependency('wasm-instantiate');

            }

        </script>

</asp:Content>

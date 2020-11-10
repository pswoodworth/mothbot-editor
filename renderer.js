// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.

const { dialog } = require("electron").remote;
const Avrgirl = require("avrgirl-arduino");
const SerialPort = require("serialport");
const Readline = require("@serialport/parser-readline");

let port;

let pathToFlash;

const chooseEl = document.querySelector("#choose-file");
const flashEl = document.querySelector("#flash");

chooseEl.addEventListener("click", function () {
  console.log("finding file...");
  const dialogPaths = dialog.showOpenDialogSync({
    properties: ["openFile"],
    filters: [{ name: "Binary Hex File", extensions: ["hex"] }],
  });
  pathToFlash = dialogPaths[0];
  if (pathToFlash) {
    console.log("PATH", flashEl);
    flashEl.disabled = false;
  }
});

flashEl.addEventListener("click", function () {
  console.log(`flashing "${pathToFlash}"...`);
  const serialPort = serialPortInput.value;
  const avrgirl = new Avrgirl({
    board: "uno",
    port: serialPort,
  });
  avrgirl.flash(pathToFlash, function (error) {
    if (error) {
      console.error(error);
    } else {
      console.info("done.");
      dialog.showMessageBoxSync({
        title: "Success!",
        message: `Successfully flashed "${pathToFlash}"!`,
      });
    }
  });
});

const connectSerialButton = document.querySelector("#connect-serial");
const disConnectSerialButton = document.querySelector("#disconnect-serial");

connectSerialButton.addEventListener("click", attachPort);

const blinkValueButton = document.querySelector("#send-blink-value");
const serialConsole = document.querySelector("#serial-console");
const serialPortInput = document.querySelector("#serial-port");

function attachPort() {
  const serialPort = serialPortInput.value;
  const baud = 9600;
  port = new SerialPort(serialPort, {
    baudRate: baud,
  });
  blinkValueButton.disabled = false;
  connectSerialButton.disabled = true;
  disConnectSerialButton.disabled = false;
  flashEl.disabled = true;
  const parser = new Readline();
  port.pipe(parser);
  parser.on("data", function (data) {
    const br = document.createElement("br");
    serialConsole.append(br);
    serialConsole.append(data);
  });
}

function disconnectSerial() {
  port.close(function () {
    blinkValueButton.disabled = true;
    connectSerialButton.disabled = false;
    disConnectSerialButton.disabled = true;
    flashEl.disabled = false;
  });
}

disConnectSerialButton.addEventListener("click", disconnectSerial);

blinkValueButton.addEventListener("click", function () {
  const value = document.querySelector("#blink-value").value;
  /*if(value==1) {
  port.write("ON\n"); 
  } else {
  port.write("OFF\n");
  }*/
  port.write(value);
  port.write("\n");
});

function setupPortSelector() {
  SerialPort.list().then((ports) => {
    ports.forEach((port) => {
      serialPortInput;
      const option = document.createElement("option");
      option.value = port.path;
      option.append(port.path);
      serialPortInput.append(option);
    });
  });
}

setupPortSelector();

// This file is required by the index.html file and will
// be executed in the renderer process for that window.
// No Node.js APIs are available in this process because
// `nodeIntegration` is turned off. Use `preload.js` to
// selectively enable features needed in the rendering
// process.

const { dialog } = require("electron").remote;
const Avrgirl = require("avrgirl-arduino");

const avrgirl = new Avrgirl({
  board: "uno",
});

let pathToFlash;

const chooseEl = document.querySelector("#choose-file");
const flashEl = document.querySelector("#flash");

chooseEl.addEventListener("click", function () {
  console.log("finding file...");
  const dialogPaths = dialog.showOpenDialogSync({
    properties: ["openFile"],
    filters: [{ name: "Binary Hex File", extensions: [".hex"] }],
  });
  pathToFlash = dialogPaths[0];
  if (pathToFlash) {
    console.log("PATH", flashEl);
    flashEl.disabled = false;
  }
});

flashEl.addEventListener("click", function () {
  console.log(`flashing "${pathToFlash}"...`);
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

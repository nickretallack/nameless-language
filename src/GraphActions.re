open Definition;
open Helpers;

type pointerID =
  | Mouse
  | Touch(int);

let pointerIDToString = (pointerID: pointerID) =>
  switch (pointerID) {
  | Mouse => "mouse"
  | Touch(id) => "touch-" ++ string_of_int(id)
  };

type drawingConnection = {
  connectionSide,
  startIsSource: bool,
  point,
};

type startDrawing = {
  pointerID,
  drawingConnection,
};

type continueDrawing = {
  pointerID,
  point,
};

type finishDrawing = {
  pointerID,
  connectionSide,
  isSource: bool,
};

type stopDrawing = {pointerID};

type graphAction =
  | StartDrawing(startDrawing)
  | ContinueDrawing(continueDrawing)
  | FinishDrawing(finishDrawing)
  | StopDrawing(stopDrawing);

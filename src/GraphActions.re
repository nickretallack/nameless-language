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

type explicitConnectionSide = {
  connectionSide,
  isSource: bool,
};

type pointerAction =
  | StartDrawing(drawingConnection)
  | ContinueDrawing(point)
  | FinishDrawing(explicitConnectionSide)
  | StopDrawing;

type graphAction = {
  pointerID,
  action: pointerAction,
};

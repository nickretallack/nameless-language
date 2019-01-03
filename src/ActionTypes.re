open Definition;

type pointerID =
  | Mouse
  | Touch(int);

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

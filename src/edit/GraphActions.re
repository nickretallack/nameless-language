[%%debugger.chrome];
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
  explicitConnectionSide,
  point,
};

type pointerState =
  | DrawingConnection(drawingConnection)
  | DraggingNode(nodeID);

type pointerAction =
  | StartDrawing(drawingConnection)
  | ContinueDrawing(point)
  | FinishDrawing(explicitConnectionSide)
  | StopDrawing
  | StartDragging(nodeID)
  | FinishDragging(nodeScope);

type pointerActionRecord = {
  pointerID,
  action: pointerAction,
};

type selectNode = {
  nodeID,
  additive: bool,
};

type graphAction =
  | PointerAction(pointerActionRecord)
  | SelectConnection(connectionSide)
  | SelectNode(selectNode)
  | RemoveSelectedNodes;

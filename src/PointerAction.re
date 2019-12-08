type t =
  | StartDrawing(DrawingConnection.t)
  | StartDragging(NodeID.t)
  | MovePointer(Point.t)
  | ReleasePointer
  | FinishDrawing(ExplicitConnectionSide.t)
  | FinishDragging(NodeScope.t);

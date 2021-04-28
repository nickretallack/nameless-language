type t =
  | DrawingConnection(DrawingConnection.t)
  | DraggingNode(NodeID.t)
  | ScrollZoom(Point.t)

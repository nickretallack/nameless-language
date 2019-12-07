type t =
  | NoSelection
  | SelectedConnection(ConnectionSide.t)
  | SelectedNib(ExplicitConnectionSide.t)
  | SelectedNodes(Belt.Set.String.t);

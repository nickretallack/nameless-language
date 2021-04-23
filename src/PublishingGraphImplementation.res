type t = {
  nodes: Belt.List.t<PublishingNode.t>,
  connections: Belt.List.t<PublishingConnection.t>,
  inputCount: int,
  outputCount: int,
}

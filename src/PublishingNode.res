type t =
  | PublishingReferenceNode
  | PublishingListNode(int)
  | PublishingDefinedNode(PublishingDefinedNode.t)

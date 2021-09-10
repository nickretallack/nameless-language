let encodeNode = (node: PublishingNode.t) => {
  open Json.Encode
  switch node {
  | PublishingListNode(length) => object_(list{("type", string("list")), ("length", int(length))})
  | PublishingDefinedNode({kind, contentID}) =>
    object_(list{
      ("type", string("defined")),
      ("kind", string(DefinedNodeKindToString.f(kind))),
      ("contentID", string(contentID)),
    })
  }
}

let encodeConnectionNode = (connectionNode: PublishingConnectionNode.t) => {
  open Json.Encode
  switch connectionNode {
  | PublishingGraphConnection => object_(list{("type", string("graph"))})
  | PublishingNodeConnection(index) =>
    object_(list{("type", string("node")), ("index", int(index))})
  }
}

let encodeConnectionNib = (connectionNib: PublishingConnectionNib.t) => {
  open Json.Encode
  switch connectionNib {
  | PublishingValueConnection => object_(list{("type", string("value"))})
  | PublishingNibConnection(index) => object_(list{("type", string("nib")), ("index", int(index))})
  }
}

let encodeConnectionSide = (connectionSide: PublishingConnectionSide.t) => {
  open Json.Encode
  object_(list{
    ("node", encodeConnectionNode(connectionSide.node)),
    ("nib", encodeConnectionNib(connectionSide.nib)),
  })
}

let encodeConnection = (connection: PublishingConnection.t) => {
  open Json.Encode
  object_(list{
    ("source", encodeConnectionSide(connection.source)),
    ("sink", encodeConnectionSide(connection.sink)),
  })
}

let encodeGraph = (graph: PublishingGraphImplementation.t) => {
  open Json.Encode
  object_(list{
    ("type", string("graph")),
    ("inputCount", int(graph.inputCount)),
    ("outputCount", int(graph.inputCount)),
    ("nodes", list(encodeNode, graph.nodes)),
    ("connections", list(encodeConnection, graph.connections)),
  })
}

let canonicalizeConnectionSide = (
  graph: GraphImplementation.t,
  dependencies: PublishingDependencyMap.t,
  nodeOrdering: list<NodeID.t>,
  graphNibOrdering: array<NodeID.t>,
  connectionSide: ConnectionSide.t,
  isSink: bool,
): PublishingConnectionSide.t =>
  switch connectionSide.node {
  | GraphConnection => {
      node: PublishingGraphConnection,
      nib: switch connectionSide.nib {
      | ValueConnection => raise(Exception.InvalidConnection)
      | PositionalConnection(_) => raise(Exception.InvalidConnection)
      | NibConnection(nibID) =>
        PublishingNibConnection(ArrayFindIndexExn.f(graphNibOrdering, nibID))
      },
    }
  | NodeConnection(nodeID) => {
      node: PublishingNodeConnection(ListFindIndexExn.f(nodeOrdering, nodeID)),
      nib: switch connectionSide.nib {
      | ValueConnection => isSink ? raise(Exception.InvalidConnection) : PublishingValueConnection
      | PositionalConnection(index) =>
        isSink ? PublishingNibConnection(index) : raise(Exception.InvalidConnection)
      | NibConnection(nibID) =>
        switch Belt.Map.String.getExn(graph.nodes, nodeID).kind {
        | ListNode(_) => raise(Exception.InvalidConnection)
        | DefinedNode({definitionID}) =>
          let dependency = Belt.Map.String.getExn(dependencies, definitionID)
          PublishingNibConnection(
            ListFindIndexExn.f(
              isSink ? dependency.inputOrdering : dependency.outputOrdering,
              nibID,
            ),
          )
        }
      },
    }
  }

let canonicalizeGraph = (
  graph: GraphImplementation.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): PublishingGraphImplementation.t => {
  let nodeOrdering = NodeInputOrdering.f(graph, dependencies, display.outputOrdering)

  {
    inputCount: Belt.Array.length(display.inputOrdering),
    outputCount: Belt.Array.length(display.outputOrdering),
    nodes: Belt.List.map(nodeOrdering, nodeID =>
      switch Belt.Map.String.getExn(graph.nodes, nodeID).kind {
      | ListNode(length) => PublishingNode.PublishingListNode(length)
      | DefinedNode({kind, definitionID}) =>
        PublishingNode.PublishingDefinedNode({
          kind: kind,
          contentID: Belt.Map.String.getExn(dependencies, definitionID).contentID,
        })
      }
    ),
    connections: Belt.List.sort(
      Belt.List.map(Belt.Map.toList(graph.connections), ((sink, source)) => {
        open PublishingConnection
        {
          sink: canonicalizeConnectionSide(
            graph,
            dependencies,
            nodeOrdering,
            display.outputOrdering,
            sink,
            true,
          ),
          source: canonicalizeConnectionSide(
            graph,
            dependencies,
            nodeOrdering,
            display.inputOrdering,
            source,
            false,
          ),
        }
      }),
      compare,
    ),
  }
}

let f = (
  graph: GraphImplementation.t,
  dependencies: PublishingDependencyMap.t,
  display: DefinitionDisplay.t,
): Js.Json.t => encodeGraph(canonicalizeGraph(graph, dependencies, display))

let encodeNode = (node: PublishingNode.t) =>
  Json.Encode.(
    switch (node) {
    | PublishingReferenceNode => object_([("type", string("reference"))])
    | PublishingListNode(length) =>
      object_([("type", string("list")), ("length", int(length))])
    | PublishingDefinedNode({kind, contentID}) =>
      object_([
        ("type", string("defined")),
        ("kind", string(DefinedNodeKindToString.f(kind))),
        ("contentID", string(contentID)),
      ])
    }
  );

let encodeConnectionNode = (connectionNode: PublishingConnectionNode.t) =>
  Json.Encode.(
    switch (connectionNode) {
    | PublishingGraphConnection => object_([("type", string("graph"))])
    | PublishingNodeConnection(index) =>
      object_([("type", string("node")), ("index", int(index))])
    }
  );

let encodeConnectionNib = (connectionNib: PublishingConnectionNib.t) =>
  Json.Encode.(
    switch (connectionNib) {
    | PublishingValueConnection => object_([("type", string("value"))])
    | PublishingNibConnection(index) =>
      object_([("type", string("nib")), ("index", int(index))])
    }
  );

let encodeConnectionSide = (connectionSide: PublishingConnectionSide.t) =>
  Json.Encode.(
    object_([
      ("node", encodeConnectionNode(connectionSide.node)),
      ("nib", encodeConnectionNib(connectionSide.nib)),
    ])
  );

let encodeConnection = (connection: PublishingConnection.t) =>
  Json.Encode.(
    object_([
      ("source", encodeConnectionSide(connection.source)),
      ("sink", encodeConnectionSide(connection.sink)),
    ])
  );

let encodeGraph = (graph: PublishingGraphImplementation.t) =>
  Json.Encode.(
    object_([
      ("type", string("graph")),
      ("inputCount", int(graph.inputCount)),
      ("outputCount", int(graph.inputCount)),
      ("nodes", list(encodeNode, graph.nodes)),
      ("connections", list(encodeConnection, graph.connections)),
    ])
  );

let canonicalizeConnectionSide =
    (
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      nodeOrdering: list(NodeID.t),
      graphNibOrdering: list(NodeID.t),
      connectionSide: ConnectionSide.t,
      isSink: bool,
    )
    : PublishingConnectionSide.t =>
  switch (connectionSide.node) {
  | GraphConnection => {
      node: PublishingGraphConnection,
      nib:
        switch (connectionSide.nib) {
        | ValueConnection => raise(Exception.InvalidConnection)
        | PositionalConnection(_) => raise(Exception.InvalidConnection)
        | NibConnection(nibID) =>
          PublishingNibConnection(
            ListFindIndexExn.f(graphNibOrdering, nibID),
          )
        },
    }
  | NodeConnection(nodeID) => {
      node:
        PublishingNodeConnection(ListFindIndexExn.f(nodeOrdering, nodeID)),
      nib:
        switch (connectionSide.nib) {
        | ValueConnection =>
          isSink
            ? raise(Exception.InvalidConnection) : PublishingValueConnection
        | PositionalConnection(index) =>
          isSink
            ? PublishingNibConnection(index)
            : raise(Exception.InvalidConnection)
        | NibConnection(nibID) =>
          switch (Belt.Map.String.getExn(graph.nodes, nodeID).kind) {
          | ReferenceNode => raise(Exception.InvalidConnection)
          | ListNode(_) => raise(Exception.InvalidConnection)
          | DefinedNode({definitionID}) =>
            let dependency =
              Belt.Map.String.getExn(dependencies, definitionID);
            PublishingNibConnection(
              ListFindIndexExn.f(
                isSink ? dependency.inputOrdering : dependency.outputOrdering,
                nibID,
              ),
            );
          }
        },
    }
  };

let canonicalizeGraph =
    (
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      display: DefinitionDisplay.t,
    )
    : PublishingGraphImplementation.t => {
  let nodeOrdering =
    NodeInputOrdering.f(graph, dependencies, display.outputOrdering);

  {
    inputCount: Belt.List.size(display.inputOrdering),
    outputCount: Belt.List.size(display.outputOrdering),
    nodes:
      Belt.List.map(nodeOrdering, nodeID =>
        switch (Belt.Map.String.getExn(graph.nodes, nodeID).kind) {
        | ReferenceNode => PublishingNode.PublishingReferenceNode
        | ListNode(length) => PublishingNode.PublishingListNode(length)
        | DefinedNode({kind, definitionID}) =>
          PublishingNode.PublishingDefinedNode({
            kind,
            contentID:
              Belt.Map.String.getExn(dependencies, definitionID).contentID,
          })
        }
      ),
    connections:
      Belt.List.sort(
        Belt.List.map(Belt.Map.toList(graph.connections), ((sink, source)) =>
          PublishingConnection.{
            sink:
              canonicalizeConnectionSide(
                graph,
                dependencies,
                nodeOrdering,
                display.outputOrdering,
                sink,
                true,
              ),
            source:
              canonicalizeConnectionSide(
                graph,
                dependencies,
                nodeOrdering,
                display.inputOrdering,
                source,
                false,
              ),
          }
        ),
        compare,
      ),
  };
};

let f =
    (
      graph: GraphImplementation.t,
      dependencies: PublishingDependencyMap.t,
      display: DefinitionDisplay.t,
    )
    : Js.Json.t =>
  encodeGraph(canonicalizeGraph(graph, dependencies, display));

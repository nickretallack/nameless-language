open Definition;

let rec findIndexExn: 'a .(Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + findIndexExn(rest, needle)
    };

let definedNodeKindToString = (kind: definedNodeKind) =>
  switch (kind) {
  | ValueNode => "value"
  | FunctionCallNode => "function call"
  | FunctionPointerCallNode => "function pointer call"
  | FunctionDefinitionNode => "function definition"
  | ConstructorNode => "constructor"
  | AccessorNode => "accessor"
  };

let encodeNode = (node: publishingNode) =>
  Json.Encode.(
    switch (node) {
    | PublishingReferenceNode => object_([("type", string("reference"))])
    | PublishingListNode(length) =>
      object_([("type", string("list")), ("length", int(length))])
    | PublishingDefinedNode({kind, contentID}) =>
      object_([
        ("type", string("defined")),
        ("kind", string(definedNodeKindToString(kind))),
        ("contentID", string(contentID)),
      ])
    }
  );

let encodeConnectionNode = (connectionNode: publishingConnectionNode) =>
  Json.Encode.(
    switch (connectionNode) {
    | PublishingGraphConnection => object_([("type", string("graph"))])
    | PublishingNodeConnection(index) =>
      object_([("type", string("node")), ("index", int(index))])
    }
  );

let encodeConnectionNib = (connectionNib: publishingConnectionNib) =>
  Json.Encode.(
    switch (connectionNib) {
    | PublishingValueConnection => object_([("type", string("value"))])
    | PublishingNibConnection(index) =>
      object_([("type", string("nib")), ("index", int(index))])
    }
  );

let encodeConnectionSide = (connectionSide: publishingConnectionSide) =>
  Json.Encode.(
    object_([
      ("node", encodeConnectionNode(connectionSide.node)),
      ("nib", encodeConnectionNib(connectionSide.nib)),
    ])
  );

let encodeConnection = (connection: publishingConnection) =>
  Json.Encode.(
    object_([
      ("source", encodeConnectionSide(connection.source)),
      ("sink", encodeConnectionSide(connection.sink)),
    ])
  );

let encodeGraph = (graph: publishingGraphImplementation) =>
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
      graph: graphImplementation,
      dependencies: publishingDependencies,
      nodeOrdering: Belt.List.t(nodeID),
      graphNibOrdering: Belt.List.t(nodeID),
      connectionSide: connectionSide,
      isSink: bool,
    )
    : publishingConnectionSide =>
  switch (connectionSide.node) {
  | GraphConnection => {
      node: PublishingGraphConnection,
      nib:
        switch (connectionSide.nib) {
        | ValueConnection => raise(InvalidConnection)
        | PositionalConnection(_) => raise(InvalidConnection)
        | NibConnection(nibID) =>
          PublishingNibConnection(findIndexExn(graphNibOrdering, nibID))
        },
    }
  | NodeConnection(nodeID) => {
      node: PublishingNodeConnection(findIndexExn(nodeOrdering, nodeID)),
      nib:
        switch (connectionSide.nib) {
        | ValueConnection =>
          isSink ? raise(InvalidConnection) : PublishingValueConnection
        | PositionalConnection(index) =>
          isSink ? PublishingNibConnection(index) : raise(InvalidConnection)
        | NibConnection(nibID) =>
          switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
          | ReferenceNode => raise(InvalidConnection)
          | ListNode(_) => raise(InvalidConnection)
          | DefinedNode({definitionID}) =>
            let dependency =
              Belt.Map.String.getExn(dependencies, definitionID);
            PublishingNibConnection(
              findIndexExn(
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
      graph: graphImplementation,
      dependencies: publishingDependencies,
      display: display,
    )
    : string => {
  let nodeOrdering =
    NodeInputOrdering.getNodeInputOrdering(
      graph,
      dependencies,
      display.outputOrdering,
    );

  Json.stringify(
    encodeGraph({
      inputCount: Belt.List.size(display.inputOrdering),
      outputCount: Belt.List.size(display.outputOrdering),
      nodes:
        Belt.List.map(nodeOrdering, nodeID =>
          switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
          | ReferenceNode => PublishingReferenceNode
          | ListNode(length) => PublishingListNode(length)
          | DefinedNode({kind, definitionID}) =>
            PublishingDefinedNode({
              kind,
              contentID:
                Belt.Map.String.getExn(dependencies, definitionID).contentID,
            })
          }
        ),
      connections:
        Belt.List.sort(
          Belt.List.map(
            Belt.Map.toList(graph.connections), ((sink, source)) =>
            {
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
    }),
  );
};

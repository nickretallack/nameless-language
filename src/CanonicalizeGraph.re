open Definition;
open OutputOrdering;
open NodeInputOrdering;

let rec findIndexExn: 'a .(Belt.List.t('a), 'a) => int =
  (list, needle) =>
    switch (list) {
    | [] => raise(Not_found)
    | [head, ...rest] => head == needle ? 0 : 1 + findIndexExn(rest, needle)
    };

let canonicalizeGraph =
    (graph: graphImplementation, dependencies: publishingDependencies)
    : publishingGraphImplementation => {
  let outputOrdering = getOutputOrdering(graph, dependencies);
  let (nodeOrdering, inputOrdering) =
    getNodeInputOrdering(graph, dependencies, outputOrdering);

  {
    inputCount: Belt.List.size(inputOrdering),
    outputCount: Belt.List.size(outputOrdering),
    nodes:
      Belt.List.map(nodeOrdering, nodeID =>
        switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
        | ReferenceNode => PublishingReferenceNode
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
        Belt.List.map(Belt.Map.toList(graph.connections), ((sink, source)) =>
          {
            sink:
              switch (sink.node) {
              | GraphConnection => {
                  node: PublishingGraphConnection,
                  nib:
                    switch (sink.nib) {
                    | ValueConnection => raise(InvalidConnection)
                    | NibConnection(nibID) =>
                      PublishingNibConnection(
                        findIndexExn(outputOrdering, nibID),
                      )
                    },
                }

              | NodeConnection(nodeID) => {
                  node:
                    PublishingNodeConnection(
                      findIndexExn(nodeOrdering, nodeID),
                    ),
                  nib:
                    switch (sink.nib) {
                    | ValueConnection => raise(InvalidConnection)
                    | NibConnection(nibID) =>
                      switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
                      | ReferenceNode => raise(InvalidConnection)
                      | DefinedNode({definitionID}) =>
                        PublishingNibConnection(
                          findIndexExn(
                            Belt.Map.String.getExn(dependencies, definitionID).
                              inputOrdering,
                            nibID,
                          ),
                        )
                      }
                    },
                }
              },
            source:
              switch (source.node) {
              | GraphConnection => {
                  node: PublishingGraphConnection,
                  nib:
                    switch (source.nib) {
                    | ValueConnection => raise(InvalidConnection)
                    | NibConnection(nibID) =>
                      PublishingNibConnection(
                        findIndexExn(inputOrdering, nibID),
                      )
                    },
                }

              | NodeConnection(nodeID) => {
                  node:
                    PublishingNodeConnection(
                      findIndexExn(nodeOrdering, nodeID),
                    ),
                  nib:
                    switch (source.nib) {
                    | ValueConnection => PublishingValueConnection
                    | NibConnection(nibID) =>
                      switch (Belt.Map.String.getExn(graph.nodes, nodeID)) {
                      | ReferenceNode => raise(InvalidConnection)
                      | DefinedNode({definitionID}) =>
                        PublishingNibConnection(
                          findIndexExn(
                            Belt.Map.String.getExn(dependencies, definitionID).
                              outputOrdering,
                            nibID,
                          ),
                        )
                      }
                    },
                }
              },
          }
        ),
        compare,
      ),
  };
};

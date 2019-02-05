[%%debugger.chrome];
open AppActions;
open! GraphActions;
open! Definition;
open Helpers;

module PointerComparator =
  Belt.Id.MakeComparable({
    type t = pointerID;
    let cmp = compare;
  });

type pointerMap('a) =
  Belt.Map.t(PointerComparator.t, 'a, PointerComparator.identity);

type graphState = {
  pointers: pointerMap(drawingConnection),
  error: option(string),
  selectedNib: option(explicitConnectionSide),
  selectedConnection: option(connectionSide),
};

let document = Webapi.Dom.Document.asEventTarget(Webapi.Dom.document);
let preventDefault = event => EventRe.preventDefault(event);
let component = ReasonReact.reducerComponent("Graph");
let make =
    (
      ~definitions: definitions,
      ~implementation: graphImplementation,
      ~definition: definition,
      ~display: display,
      ~documentation: documentation,
      ~emit: definitionAction => unit,
      _children,
    ) => {
  ...component,
  /* Prevent iOS from scrolling all over the place */
  didMount: _ =>
    Webapi.Dom.EventTarget.addEventListenerWithOptions(
      "touchmove",
      preventDefault,
      {"passive": false, "capture": true, "once": false},
      document,
    ),
  willUnmount: _ =>
    Webapi.Dom.EventTarget.removeEventListener(
      "touchmove",
      preventDefault,
      document,
    ),
  initialState: () => {
    pointers: Belt.Map.make(~id=(module PointerComparator)),
    error: None,
    selectedNib: None,
    selectedConnection: None,
  },
  reducer: (action: graphAction, state: graphState) =>
    switch (action) {
    | SelectConnection(connectionSide) =>
      ReasonReact.Update({
        ...state,
        selectedConnection: Some(connectionSide),
      })
    | PointerAction({pointerID, action}) =>
      switch (action) {
      | StartDrawing(drawingConnection) =>
        ReasonReact.Update({
          ...state,
          pointers:
            Belt.Map.set(state.pointers, pointerID, drawingConnection),
        })
      | ContinueDrawing(point) =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | Some(drawingConnection) =>
          ReasonReact.Update({
            ...state,
            pointers:
              Belt.Map.set(
                state.pointers,
                pointerID,
                {...drawingConnection, point},
              ),
          })
        | None => ReasonReact.NoUpdate
        }
      | FinishDrawing({connectionSide: endNib, isSource: endIsSource}) =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | None => ReasonReact.NoUpdate
        | Some({
            explicitConnectionSide: {
              isSource: startIsSource,
              connectionSide: startNib,
            },
          }) =>
          if (startIsSource == endIsSource) {
            if (startNib == endNib) {
              ReasonReact.Update({
                ...state,
                error: None,
                selectedConnection: None,
                selectedNib:
                  Some({connectionSide: startNib, isSource: startIsSource}),
              });
            } else {
              ReasonReact.Update({
                ...state,
                error:
                  Some(
                    startIsSource ?
                      "Can't connect a source to a source" :
                      "Can't connect a sink to a sink",
                  ),
              });
            };
          } else {
            let (source, sink) =
              startIsSource ? (startNib, endNib) : (endNib, startNib);
            if (!DetectCycles.checkScopes(source, sink, implementation.nodes)) {
              ReasonReact.Update({
                ...state,
                error:
                  Some(
                    "When crossing scopes, you can only connect a source in a parent scope to a sink in a child scope.",
                  ),
              });
            } else if (DetectCycles.detectCycles(
                         Belt.Map.set(
                           implementation.connections,
                           sink,
                           source,
                         ),
                         implementation.nodes,
                       )) {
              ReasonReact.Update({
                ...state,
                error: Some("Can't create cycles"),
              });
            } else {
              ReasonReact.UpdateWithSideEffects(
                {
                  ...state,
                  error: None,
                  pointers: Belt.Map.remove(state.pointers, pointerID),
                },
                _ => emit(CreateConnection({source, sink})),
              );
            };
          }
        }
      | StopDrawing =>
        Belt.Map.has(state.pointers, pointerID) ?
          ReasonReact.Update({
            ...state,
            pointers: Belt.Map.remove(state.pointers, pointerID),
          }) :
          ReasonReact.NoUpdate
      }
    },
  render: self => {
    Js.log(encodeGraphImplementation(implementation));

    let getNode = (nodeID: nodeID) =>
      Belt.Map.String.getExn(implementation.nodes, nodeID);

    let columns: list(nodes) =
      ColumnizeNodes.columnizeNodes(
        implementation.nodes,
        implementation.connections,
      );

    /* TODO: ColumnizeNodes should output this format */
    let columnizedNodes =
      Belt.List.map(columns, nodes =>
        Belt.List.map(
          Belt.List.fromArray(Belt.Map.String.toArray(nodes)), ((id, node)) =>
          {id, node}
        )
      );

    let scopedNodeIDs =
      Belt.Map.String.reduce(
        implementation.nodes,
        Belt.Map.make(~id=(module ScopeComparator)),
        (scopes, id, node) =>
        Belt.Map.update(scopes, node.scope, nodeIDs =>
          Some(
            Belt.Set.String.add(
              switch (nodeIDs) {
              | None => Belt.Set.String.empty
              | Some(nodeIDs) => nodeIDs
              },
              id,
            ),
          )
        )
      );

    let (nodeLayouts, graphSize) =
      LayoutGraph.layoutGraph(
        scopedNodeIDs,
        columnizedNodes,
        definitions,
        implementation.connections,
      );

    let graphSize: LayoutGraph.nodePosition = {
      columns: graphSize.columns,
      rows:
        max(
          graphSize.rows,
          max(
            Belt.List.length(definition.display.inputOrdering),
            Belt.List.length(definition.display.outputOrdering),
          ),
        ),
    };

    let nodeWidth = 120.0;
    let textHeight = 20.0;
    let xPadding = 60.0;
    /* let yPadding = 20.0; */
    let columnWidth = nodeWidth +. xPadding;
    let getNodePosition = nodeID => {
      let position = Belt.Map.String.getExn(nodeLayouts, nodeID).position;
      {
        x: float_of_int(position.columns + 1) *. columnWidth,
        y: float_of_int(position.rows) *. textHeight,
      };
    };

    let sizeToPixels = (size: LayoutGraph.nodePosition) => {
      x: float_of_int(size.columns) *. columnWidth -. xPadding,
      y: float_of_int(size.rows) *. textHeight,
    };

    let graphSizePixels = sizeToPixels(graphSize);

    let getNodeSize = nodeID =>
      sizeToPixels(Belt.Map.String.getExn(nodeLayouts, nodeID).size);
    /*
     let getNodeDepth = nodeID =>
       Belt.Map.String.getExn(nodeLayouts, nodeID).depth; */

    let isNibInternal = (node: node, nib: connectionNib) =>
      switch (node.kind) {
      | DefinedNode({kind: FunctionDefinitionNode}) =>
        switch (nib) {
        | NibConnection(_) => true
        | _ => false
        }
      | _ => false
      };

    let nibPositionFormula =
        (isInternal, isSink, nodePosition, nodeSize, nibIndex) => {
      let rightSide = isInternal ? !isSink : isSink;
      {
        x:
          nodePosition.x
          +. (
            isInternal ?
              rightSide ? nodeSize.x -. nodeWidth : nodeWidth :
              rightSide ? nodeSize.x : 0.0
          ),
        y:
          float_of_int(nibIndex)
          *. textHeight
          +. textHeight
          /. 2.0
          +. nodePosition.y,
      };
    };

    let getNibPosition = (connectionSide: connectionSide, isSink: bool) =>
      switch (connectionSide.node) {
      | NodeConnection(nodeID) =>
        let nodePosition = getNodePosition(nodeID);
        let nodeSize = getNodeSize(nodeID);
        let node = getNode(nodeID);
        let isInternal = isNibInternal(node, connectionSide.nib);
        let nibIndex =
          getNodeNibIndex(node, definitions, connectionSide.nib, isSink);
        nibPositionFormula(
          isInternal,
          isSink,
          nodePosition,
          nodeSize,
          nibIndex,
        );
      | GraphConnection =>
        let nibIndex =
          findByIndexExn(
            isSink ?
              definition.display.outputOrdering :
              definition.display.inputOrdering,
            nibID =>
            NibConnection(nibID) == connectionSide.nib
          );
        nibPositionFormula(
          true,
          isSink,
          {x: 0.0, y: textHeight},
          graphSizePixels,
          nibIndex,
        );
      };

    let getNibNudge = (source: connectionSide) =>
      switch (source.node) {
      | NodeConnection(nodeID) =>
        let node = getNode(nodeID);
        getOutputIndex(node, definitions, source.nib);
      | GraphConnection =>
        switch (source.nib) {
        | NibConnection(nibID) => findIndexExn(display.inputOrdering, nibID)
        | _ => raise(Not_found)
        }
      };

    /* let evaluate = outputID =>
       Js.log(
         Evaluate.evaluateGraphOutput(definitions, implementation, outputID),
       ); */

    let allNibs = collectAllGraphNibs(definition, definitions);

    let renderedConnections =
      renderMap(
        ((sink, source)) =>
          <SvgConnection
            key={connectionSideToString(sink)}
            sinkPosition={getNibPosition(sink, true)}
            sourcePosition={getNibPosition(source, false)}
            nudge={getNibNudge(source)}
            onClick={_event => self.send(SelectConnection(sink))}
            isSelected={self.state.selectedConnection == Some(sink)}
          />,
        implementation.connections,
      );
    let renderedNibs =
      ReasonReact.array(
        Belt.Array.map(
          Belt.List.toArray(allNibs),
          ({name, explicitConnectionSide}) => {
            let {connectionSide, isSource} = explicitConnectionSide;
            <SvgNib
              key={explicitConnectionSideKey(explicitConnectionSide)}
              text=name
              isSource
              connectionSide
              position={getNibPosition(connectionSide, !isSource)}
              emit={self.send}
              isHighlighted={
                switch (self.state.selectedNib) {
                | None => false
                | Some(highlightedExplicitConnectionSide) =>
                  highlightedExplicitConnectionSide == explicitConnectionSide
                }
              }
            />;
          },
        ),
      );

    let renderedSides =
      <SvgDefinitionBox
        name={getDisplayName(definition, "en")}
        position={x: 0.0, y: 0.0}
        size=graphSizePixels
        nodeWidth
        textHeight
      />;

    let renderedNodes =
      ReasonReact.array(
        Belt.List.toArray(
          Belt.List.map(
            Belt.Map.String.toList(implementation.nodes), ((nodeID, node)) =>
            <SvgNode
              key=nodeID
              node
              definitions
              position={getNodePosition(nodeID)}
              size={getNodeSize(nodeID)}
              nodeWidth
              textHeight
            />
          ),
        ),
      );

    let renderedDrawingConnections =
      renderMap(
        (
          (
            pointerID: pointerID,
            {
              point,
              explicitConnectionSide: {
                connectionSide,
                isSource: startIsSource,
              },
            }: drawingConnection,
          ),
        ) => {
          let adjustedPoint = {x: point.x, y: point.y -. 40.0};
          <SvgConnection
            key={pointerIDToString(pointerID)}
            sourcePosition={
              startIsSource ?
                getNibPosition(connectionSide, false) : adjustedPoint
            }
            sinkPosition={
              startIsSource ?
                adjustedPoint : getNibPosition(connectionSide, true)
            }
          />;
        },
        self.state.pointers,
      );

    <div>
      <svg
        width={pixels(graphSizePixels.x)}
        height={pixels(graphSizePixels.y)}
        onMouseMove={event => {
          ReactEvent.Mouse.preventDefault(event);
          self.send(
            PointerAction({
              pointerID: Mouse,
              action: ContinueDrawing(pointFromMouse(event)),
            }),
          );
        }}
        onTouchMove={event =>
          iterateTouches(event, touch =>
            self.send(
              PointerAction({
                pointerID: Touch(touch##identifier),
                action:
                  ContinueDrawing({
                    x:
                      touch##clientX
                      -.
                      ReactEvent.Touch.currentTarget(event)##offsetLeft,
                    y:
                      touch##clientY
                      -.
                      ReactEvent.Touch.currentTarget(event)##offsetTop,
                  }),
              }),
            )
          )
        }
        onMouseUp={_ =>
          self.send(PointerAction({pointerID: Mouse, action: StopDrawing}))
        }
        onTouchEnd={event =>
          iterateTouches(event, touch =>
            self.send(
              PointerAction({
                pointerID: Touch(touch##identifier),
                action: StopDrawing,
              }),
            )
          )
        }>
        renderedSides
        renderedNodes
        renderedConnections
        renderedDrawingConnections
        renderedNibs
      </svg>
      {switch (self.state.error) {
       | Some(error) =>
         <div className="error-message"> {ReasonReact.string(error)} </div>
       | None => ReasonReact.null
       }}
      {switch (self.state.selectedNib) {
       | None => ReasonReact.null
       | Some(explicitConnectionSide) =>
         <NodeMenu
           emit
           definitions
           nodes={implementation.nodes}
           nib=explicitConnectionSide
         />
       }}
      <DefinitionHeader documentation emit placeholder="(nameless graph)" />
      <h2> {ReasonReact.string("Interface")} </h2>
      <Interface
        definitions
        interface={implementation.interface}
        documentation
        display
        emit
      />
    </div>;
  },
};

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

type selection =
  | NoSelection
  | SelectedConnection(connectionSide)
  | SelectedNib(explicitConnectionSide)
  | SelectedNodes(Belt.Set.String.t);

type graphState = {
  pointers: pointerMap(pointerState),
  error: option(string),
  selection,
};

let document = Webapi.Dom.Document.asEventTarget(Webapi.Dom.document);
let preventDefault = event => EventRe.preventDefault(event);
let component = ReasonReact.reducerComponent("Graph");
let make =
    (
      ~definitionID: definitionID,
      ~definitions: definitions,
      ~implementation: graphImplementation,
      ~definition: definition,
      ~display: display,
      ~documentation: documentation,
      ~emit: definitionAction => unit,
      ~error: AppActions.appError,
      _children,
    ) => {
  ...component,
  /* Prevent iOS from scrolling all over the place */
  didMount: _ => {
    Webapi.Dom.EventTarget.addEventListenerWithOptions(
      "touchmove",
      preventDefault,
      {"passive": false, "capture": true, "once": false},
      document,
    );
  },
  willUnmount: _ =>
    Webapi.Dom.EventTarget.removeEventListener(
      "touchmove",
      preventDefault,
      document,
    ),
  initialState: () => {
    pointers: Belt.Map.make(~id=(module PointerComparator)),
    error: None,
    selection: NoSelection,
  },
  reducer: (action: graphAction, state: graphState) =>
    switch (action) {
    | SelectNode({nodeID, additive}) =>
      ReasonReact.Update({
        ...state,
        selection:
          switch (state.selection) {
          | SelectedNodes(nodeIDs) =>
            if (additive) {
              if (Belt.Set.String.has(nodeIDs, nodeID)) {
                let newNodeIDs = Belt.Set.String.remove(nodeIDs, nodeID);
                if (Belt.Set.String.isEmpty(newNodeIDs)) {
                  NoSelection;
                } else {
                  SelectedNodes(newNodeIDs);
                };
              } else {
                SelectedNodes(Belt.Set.String.add(nodeIDs, nodeID));
              };
            } else {
              SelectedNodes(Belt.Set.String.fromArray([|nodeID|]));
            }
          | _ => SelectedNodes(Belt.Set.String.fromArray([|nodeID|]))
          },
      })
    | RemoveSelectedNodes =>
      switch (state.selection) {
      | SelectedNodes(nodeIDs) =>
        ReasonReact.UpdateWithSideEffects(
          {...state, selection: NoSelection},
          _ => emit(RemoveNodes(nodeIDs)),
        )
      | _ => ReasonReact.NoUpdate
      }
    | SelectConnection(connectionSide) =>
      ReasonReact.Update({
        ...state,
        selection: SelectedConnection(connectionSide),
      })
    | PointerAction({pointerID, action}) =>
      switch (action) {
      | StartDrawing(drawingConnection) =>
        ReasonReact.Update({
          ...state,
          pointers:
            Belt.Map.set(
              state.pointers,
              pointerID,
              DrawingConnection(drawingConnection),
            ),
        })
      | StartDragging(nodeID) =>
        ReasonReact.Update({
          ...state,
          pointers:
            Belt.Map.set(state.pointers, pointerID, DraggingNode(nodeID)),
        })
      | MovePointer(point) =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | Some(DrawingConnection(drawingConnection)) =>
          ReasonReact.Update({
            ...state,
            pointers:
              Belt.Map.set(
                state.pointers,
                pointerID,
                DrawingConnection({...drawingConnection, point}),
              ),
          })
        | _ => ReasonReact.NoUpdate
        }
      | FinishDragging(nodeScope) =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | Some(DraggingNode(nodeID)) =>
          ReasonReact.SideEffects(
            _ => emit(ChangeNodeScope({nodeID, nodeScope})),
          )
        | _ => ReasonReact.NoUpdate
        }
      | FinishDrawing({connectionSide: endNib, isSource: endIsSource}) =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | Some(
            DrawingConnection({
              explicitConnectionSide: {
                isSource: startIsSource,
                connectionSide: startNib,
              },
            }),
          ) =>
          if (startIsSource == endIsSource) {
            if (startNib == endNib) {
              ReasonReact.Update({
                ...state,
                error: None,
                selection:
                  SelectedNib({
                    connectionSide: startNib,
                    isSource: startIsSource,
                  }),
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
        | _ => ReasonReact.NoUpdate
        }

      | ReleasePointer =>
        switch (Belt.Map.get(state.pointers, pointerID)) {
        | Some(DrawingConnection(_)) =>
          ReasonReact.Update({
            ...state,
            pointers: Belt.Map.remove(state.pointers, pointerID),
          })
        | Some(DraggingNode(nodeID)) =>
          ReasonReact.UpdateWithSideEffects(
            {...state, pointers: Belt.Map.remove(state.pointers, pointerID)},
            _ => emit(ChangeNodeScope({nodeID, nodeScope: GraphScope})),
          )
        | _ => ReasonReact.NoUpdate
        }
      }
    },
  render: self => {
    let getNode = (nodeID: nodeID) =>
      Belt.Map.String.getExn(implementation.nodes, nodeID);

    let columnizedNodes =
      ColumnizeNodes.columnizeNodes(
        implementation.nodes,
        implementation.connections,
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

    let getNodeDepth = nodeID =>
      Belt.Map.String.getExn(nodeLayouts, nodeID).depth;

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
            isSelected={self.state.selection == SelectedConnection(sink)}
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
                switch (self.state.selection) {
                | SelectedNib(highlightedExplicitConnectionSide) =>
                  highlightedExplicitConnectionSide == explicitConnectionSide
                | _ => false
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
            sortBy(
              Belt.Map.String.toList(implementation.nodes), ((nodeID, _)) =>
              getNodeDepth(nodeID)
            ),
            ((nodeID, node)) =>
            <SvgNode
              key=nodeID
              node
              definitions
              position={getNodePosition(nodeID)}
              size={getNodeSize(nodeID)}
              nodeWidth
              textHeight
              selected={
                switch (self.state.selection) {
                | SelectedNodes(nodeIDs) =>
                  Belt.Set.String.has(nodeIDs, nodeID)
                | _ => false
                }
              }
              onClick={event =>
                self.send(
                  SelectNode({
                    nodeID,
                    additive: ReactEvent.Mouse.shiftKey(event),
                  }),
                )
              }
              onMouseDown={_ =>
                self.send(
                  PointerAction({
                    pointerID: Mouse,
                    action: StartDragging(nodeID),
                  }),
                )
              }
              onMouseUp={_ =>
                self.send(
                  PointerAction({
                    pointerID: Mouse,
                    action: FinishDragging(NodeScope(nodeID)),
                  }),
                )
              }
            />
          ),
        ),
      );

    let renderedDrawingConnections =
      renderMap(
        ((pointerID: pointerID, pointerState: pointerState)) =>
          switch (pointerState) {
          | DrawingConnection({
              point,
              explicitConnectionSide: {
                connectionSide,
                isSource: startIsSource,
              },
            }) =>
            let adjustedPoint = {x: point.x, y: point.y -. 18.0};
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
          | _ => ReasonReact.null
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
              action: MovePointer(pointFromMouse(event)),
            }),
          );
        }}
        onTouchMove={event =>
          iterateTouches(event, touch =>
            self.send(
              PointerAction({
                pointerID: Touch(touch##identifier),
                action:
                  MovePointer({
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
          self.send(
            PointerAction({pointerID: Mouse, action: ReleasePointer}),
          )
        }
        onTouchEnd={event =>
          iterateTouches(event, touch =>
            self.send(
              PointerAction({
                pointerID: Touch(touch##identifier),
                action: ReleasePointer,
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
      <DefinitionHeader
        definitionID
        definitions
        documentation
        emit
        error
        placeholder="(nameless graph)"
      />
      {switch (self.state.error) {
       | Some(error) =>
         <div className="error-message"> {ReasonReact.string(error)} </div>
       | None => ReasonReact.null
       }}
      {switch (self.state.selection) {
       | SelectedNib(explicitConnectionSide) =>
         <NodeMenu
           emit
           definitions
           nodes={implementation.nodes}
           nib=explicitConnectionSide
         />
       | SelectedConnection(connectionSide) =>
         <button onClick={_event => emit(RemoveConnection(connectionSide))}>
           {ReasonReact.string("Remove connection")}
         </button>
       | SelectedNodes(_) =>
         <button onClick={_event => self.send(RemoveSelectedNodes)}>
           {ReasonReact.string("Remove Node(s)")}
         </button>
       | NoSelection => ReasonReact.null
       }}
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

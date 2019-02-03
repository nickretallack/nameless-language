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
};

let renderNibs =
    (
      nibs: list(displayNib),
      className: string,
      isSource: bool,
      connectionNode: connectionNode,
      emit: GraphActions.graphAction => unit,
      appEmit: AppActions.definitionAction => unit,
      selectedNib: option(connectionNib),
    ) => {
  ReasonReact.array(
    Array.of_list(
      Belt.List.map(
        nibs,
        ({name, nib}) => {
          let nameEditor =
            <input
              value=name
              onChange={event =>
                appEmit(
                  AppActions.NibAction({
                    isInput: isSource,
                    nibID:
                      switch (nib) {
                      | NibConnection(nibID) => nibID
                      | _ => raise(Not_found)
                      },
                    action: ChangeNibName(getEventValue(event)),
                  }),
                )
              }
            />;
          <div className key={SimpleNode.nibKey(nib)}>
            {isSource ? ReasonReact.null : nameEditor}
            <Nib
              isSource
              isHighlighted={Some(nib) == selectedNib}
              connectionSide={node: connectionNode, nib}
              emit
            />
            {isSource ? nameEditor : ReasonReact.null}
          </div>;
        },
      ),
    ),
  );
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
  },
  reducer: ({pointerID, action}: graphAction, state: graphState) =>
    switch (action) {
    | StartDrawing(drawingConnection) =>
      ReasonReact.Update({
        ...state,
        pointers: Belt.Map.set(state.pointers, pointerID, drawingConnection),
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
                       Belt.Map.set(implementation.connections, sink, source),
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
    },
  render: self => {
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
          {x: 0.0, y: 0.0},
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

    let changeName = event => emit(ChangeName(getEventValue(event)));

    let selectedGraphNib = isSource =>
      switch (self.state.selectedNib) {
      | None => None
      | Some({connectionSide, isSource: selectionIsSource}) =>
        isSource != selectionIsSource ?
          None :
          (
            switch (connectionSide.node) {
            | NodeConnection(_) => None
            | GraphConnection => Some(connectionSide.nib)
            }
          )
      };

    let selectedGraphInputNib = selectedGraphNib(true);
    let selectedGraphOutputNib = selectedGraphNib(false);

    /* let evaluate = outputID =>
       Js.log(
         Evaluate.evaluateGraphOutput(definitions, implementation, outputID),
       ); */

    /* let renderNibs = (nibs, isSink) =>
       ReasonReact.array(Belt.List.map */

    let allNibs = collectAllGraphNibs(definition, definitions);

    let renderedConnections =
      renderMap(
        ((sink, source)) =>
          <SvgConnection
            key={connectionSideToString(sink)}
            sinkPosition={getNibPosition(sink, true)}
            sourcePosition={getNibPosition(source, false)}
            nudge={getNibNudge(source)}
          />,
        implementation.connections,
      );
    let renderedNibs =
      ReasonReact.array(
        Belt.Array.map(
          Belt.List.toArray(allNibs),
          (explicitConnectionSide: explicitConnectionSide) => {
            let {connectionSide, isSource} = explicitConnectionSide;
            <SvgNib
              key={SimpleNode.explicitConnectionSideKey(
                explicitConnectionSide,
              )}
              isSource
              connectionSide
              position={getNibPosition(connectionSide, !isSource)}
              emit={self.send}
              isHighlighted=false
            />;
          },
        ),
      );

    let renderedSides =
      <>
        <NibNames
          position={x: 0.0, y: 0.0}
          height={graphSizePixels.y}
          sinks={displayKeywordOutputs(definition, "en")}
          nodeWidth
          textHeight
        />
        <NibNames
          position={x: graphSizePixels.x -. nodeWidth, y: 0.0}
          height={graphSizePixels.y}
          sources={displayKeywordInputs(definition, "en")}
          nodeWidth
          textHeight
        />
      </>;

    <div>
      <svg
        width={pixels(graphSizePixels.x)} height={pixels(graphSizePixels.y)}>
        renderedSides
        renderedConnections
        renderedNibs
      </svg>
      <input
        type_="text"
        className="graph-name"
        placeholder="(nameless function)"
        value={getTranslated(documentation.name, "en")}
        onChange=changeName
      />
      <div
        className="graph"
        style={ReactDOMRe.Style.make(
          ~width=pixels(graphSizePixels.x),
          ~height=pixels(graphSizePixels.y),
          (),
        )}
        onMouseMove={event => {
          ReactEvent.Mouse.preventDefault(event);
          self.send({
            pointerID: Mouse,
            action: ContinueDrawing(pointFromMouse(event)),
          });
        }}
        onTouchMove={event =>
          iterateTouches(event, touch =>
            self.send({
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
            })
          )
        }
        onMouseUp={_ => self.send({pointerID: Mouse, action: StopDrawing})}
        onTouchEnd={event =>
          iterateTouches(event, touch =>
            self.send({
              pointerID: Touch(touch##identifier),
              action: StopDrawing,
            })
          )
        }>
        {renderMap(
           ((sink, source)) =>
             <Connection
               key={connectionSideToString(sink)}
               sinkPosition={getNibPosition(sink, true)}
               sourcePosition={getNibPosition(source, false)}
               nudge={getNibNudge(source)}
             />,
           implementation.connections,
         )}
        {renderMap(
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
             <Connection
               key={pointerIDToString(pointerID)}
               sourcePosition={
                 startIsSource ?
                   getNibPosition(connectionSide, false) : adjustedPoint
               }
               sinkPosition={
                 startIsSource ?
                   adjustedPoint : getNibPosition(connectionSide, true)
               }
               nudge={startIsSource ? getNibNudge(connectionSide) : 0}
             />;
           },
           self.state.pointers,
         )}
        <div className="outputs">
          {renderNibs(
             displayKeywordOutputs(definition, "en"),
             "output internal",
             false,
             GraphConnection,
             self.send,
             emit,
             selectedGraphOutputNib,
           )}
          <a onClick={_event => emit(AddOutput)}>
            {ReasonReact.string("Add Output")}
          </a>
        </div>
        <div className="inputs">
          {renderNibs(
             displayKeywordInputs(definition, "en"),
             "input internal",
             true,
             GraphConnection,
             self.send,
             emit,
             selectedGraphInputNib,
           )}
          <a onClick={_event => emit(AddInput)}>
            {ReasonReact.string("Add Input")}
          </a>
        </div>
        {renderStringMap(
           ((nodeID: nodeID, node: node)) =>
             <Node
               key=nodeID
               nodeID
               definitions
               node
               selectedNib=?{
                 switch (self.state.selectedNib) {
                 | None => None
                 | Some({connectionSide}) =>
                   switch (connectionSide.node) {
                   | GraphConnection => None
                   | NodeConnection(selectedNodeID) =>
                     selectedNodeID == nodeID ?
                       Some(connectionSide.nib) : None
                   }
                 }
               }
               position={getNodePosition(nodeID)}
               size={getNodeSize(nodeID)}
               depth={getNodeDepth(nodeID)}
               emit={self.send}
             />,
           implementation.nodes,
         )}
      </div>
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
    </div>;
  },
};

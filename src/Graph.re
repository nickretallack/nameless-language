[%%debugger.chrome];
open AppActions;
open GraphActions;
open Definition;
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
};

let document = Webapi.Dom.Document.asEventTarget(Webapi.Dom.document);
let preventDefault = event => EventRe.preventDefault(event);
let component = ReasonReact.reducerComponent("Graph");
let make =
    (
      ~definitionID: definitionID,
      ~definitions: definitions,
      ~implementation: graphImplementation,
      ~display: display,
      ~documentation: documentation,
      ~size: point,
      ~emit: appAction => unit,
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
    error: None,
    pointers: Belt.Map.make(~id=(module PointerComparator)),
  },
  reducer: (action: graphAction, state: graphState) =>
    switch (action) {
    | StartDrawing({pointerID, drawingConnection}) =>
      ReasonReact.Update({
        ...state,
        pointers: Belt.Map.set(state.pointers, pointerID, drawingConnection),
      })
    | ContinueDrawing({pointerID, point}) =>
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
    | FinishDrawing({pointerID, connectionSide: end_nib, isSource}) =>
      switch (Belt.Map.get(state.pointers, pointerID)) {
      | Some({startIsSource, connectionSide: start_nib}) =>
        startIsSource != isSource ?
          DetectCycles.detectCycles(
            Belt.Map.set(
              implementation.connections,
              startIsSource ? end_nib : start_nib,
              startIsSource ? start_nib : end_nib,
            ),
            implementation.nodes,
          ) ?
            ReasonReact.Update({
              ...state,
              error: Some("Can't create cycles"),
            }) :
            ReasonReact.UpdateWithSideEffects(
              {
                error: None,
                pointers: Belt.Map.remove(state.pointers, pointerID),
              },
              _ =>
                emit(
                  DefinitionAction({
                    definitionID,
                    action:
                      CreateConnection({
                        source: startIsSource ? start_nib : end_nib,
                        sink: startIsSource ? end_nib : start_nib,
                      }),
                  }),
                ),
            ) :
          ReasonReact.Update({
            ...state,
            error:
              Some(
                startIsSource ?
                  "Can't connect a source to a source" :
                  "Can't connect a sink to a sink",
              ),
          })

      | None => ReasonReact.NoUpdate
      }
    | StopDrawing({pointerID}) =>
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
      ColumnizeNodes.topoSort(
        implementation.nodes,
        implementation.connections,
      );
    let columnWidth = size.x /. float_of_int(List.length(columns) + 1);
    let nodeWidth = 80.0;
    let textHeight = 20.0;
    let nodeHeight = (node: node) =>
      textHeight *. float_of_int(1 + countNodeNibs(node, definitions));

    let nodePositions: Belt.Map.String.t(point) =
      Belt.Map.String.mergeMany(
        Belt.Map.String.empty,
        Array.of_list(
          List.flatten(
            List.mapi(
              (column, nodes: Belt.Map.String.t(node)) => {
                let rowHeight =
                  size.y /. float_of_int(Belt.Map.String.size(nodes) + 1);
                Belt.List.mapWithIndex(
                  Belt.Map.String.toList(nodes),
                  (row, (nodeID: nodeID, node: node)) =>
                  (
                    nodeID,
                    {
                      x:
                        columnWidth
                        *. float_of_int(column + 1)
                        -. nodeWidth
                        /. 2.0,
                      y:
                        rowHeight
                        *. float_of_int(row + 1)
                        -. nodeHeight(node)
                        /. 2.0,
                    },
                  )
                );
              },
              columns,
            ),
          ),
        ),
      );
    let getNodePosition = nodeID =>
      Belt.Map.String.getExn(nodePositions, nodeID);

    let nibOffset = 10.0;
    let nibPositions = (nibIds, isInput) => {
      let rowHeight = size.y /. float_of_int(List.length(nibIds) + 1);
      Belt.Map.String.fromArray(
        Array.of_list(
          List.mapi(
            (index, nibID) =>
              (
                nibID,
                {
                  x:
                    if (isInput) {
                      size.x -. nibOffset;
                    } else {
                      nibOffset;
                    },
                  y: float_of_int(index + 1) *. rowHeight,
                },
              ),
            nibIds,
          ),
        ),
      );
    };
    let inputPositions = nibPositions(display.inputOrdering, true);
    let outputPositions = nibPositions(display.outputOrdering, false);

    let getNibPosition = (connectionSide: connectionSide, isSink: bool) =>
      switch (connectionSide.node) {
      | NodeConnection(nodeID) =>
        let nodePosition = getNodePosition(nodeID);
        let node = getNode(nodeID);
        {
          x: nodePosition.x +. (isSink ? 80.0 : 0.0),
          y:
            float_of_int(
              getNodeNibIndex(node, definitions, connectionSide.nib, isSink)
              + 1,
            )
            *. textHeight
            +. textHeight
            /. 2.0
            +. nodePosition.y,
        };
      | GraphConnection =>
        switch (connectionSide.nib) {
        | NibConnection(nibID) =>
          Belt.Map.String.getExn(
            isSink ? outputPositions : inputPositions,
            nibID,
          )
        | _ => raise(Not_found)
        }
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

    let changeName = event =>
      emit(
        DefinitionAction({
          definitionID,
          action: ChangeName(getEventValue(event)),
        }),
      );

    /* let evaluate = output_id =>
       Js.log(evaluateOutput(definitions, definitionID, output_id)); */
    <div
      className="graph"
      onMouseMove={event => {
        ReactEvent.Mouse.preventDefault(event);
        self.send(
          ContinueDrawing({pointerID: Mouse, point: pointFromMouse(event)}),
        );
      }}
      onTouchMove={event =>
        iterateTouches(event, touch =>
          self.send(
            ContinueDrawing({
              pointerID: Touch(touch##identifier),
              point: {
                x: touch##clientX,
                y: touch##clientY,
              },
            }),
          )
        )
      }
      onMouseUp={_ => self.send(StopDrawing({pointerID: Mouse}))}
      onTouchEnd={event =>
        iterateTouches(event, touch =>
          self.send(StopDrawing({pointerID: Touch(touch##identifier)}))
        )
      }>
      <input
        type_="text"
        className="graph-name"
        value={
                Belt.Map.String.getExn(documentation.name.translations, "en").
                  text
              }
        onChange=changeName
      />
      {switch (self.state.error) {
       | Some(error) =>
         <div className="error-message"> {ReasonReact.string(error)} </div>
       | None => ReasonReact.null
       }}
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
             {connectionSide, startIsSource, point}: drawingConnection,
           ),
         ) =>
           <Connection
             key={pointerIDToString(pointerID)}
             sourcePosition={
               startIsSource ? getNibPosition(connectionSide, false) : point
             }
             sinkPosition={
               startIsSource ? point : getNibPosition(connectionSide, true)
             }
             nudge={startIsSource ? getNibNudge(connectionSide) : 0}
           />,
         self.state.pointers,
       )}
      {ReasonReact.array(
         Belt.List.toArray(
           Belt.List.map(
             display.inputOrdering,
             (nibID: nibID) => {
               let name =
                 getTranslated(
                   Belt.Map.String.getExn(documentation.inputs, nibID),
                   "en",
                 );
               <div
                 className="graph-input input"
                 key=nibID
                 style={ReactDOMRe.Style.make(
                   ~right=pixels(10.0),
                   ~top=
                     pixels(Belt.Map.String.getExn(inputPositions, nibID).y),
                   (),
                 )}>
                 {ReasonReact.string(name)}
                 <Nib
                   isSource=true
                   connectionSide={
                     node: GraphConnection,
                     nib: NibConnection(nibID),
                   }
                   emit={self.send}
                 />
               </div>;
             },
           ),
         ),
       )}
      /* <a onClick={_ => emit(AddInput({definitionID: definitionID}))}>
           {ReasonReact.string("Add Input")}
         </a> */
      {ReasonReact.array(
         Belt.List.toArray(
           Belt.List.map(
             display.outputOrdering,
             (nibID: nibID) => {
               let name =
                 getTranslated(
                   Belt.Map.String.getExn(documentation.outputs, nibID),
                   "en",
                 );

               <div
                 className="graph-output output"
                 key=nibID
                 style={positionStyle(
                   Belt.Map.String.getExn(outputPositions, nibID),
                 )}>
                 <Nib
                   isSource=false
                   connectionSide={
                     node: GraphConnection,
                     nib: NibConnection(nibID),
                   }
                   emit={self.send}
                 />
                 {ReasonReact.string(name)}
               </div>;
             },
           ),
         ),
       )}
      {renderStringMap(
         ((nodeID: nodeID, node: node)) =>
           <Node
             key=nodeID
             nodeID
             definitions
             node
             position={Belt.Map.String.getExn(nodePositions, nodeID)}
             emit={self.send}
           />,
         implementation.nodes,
       )}
    </div>;
  },
};

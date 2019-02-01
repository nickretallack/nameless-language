open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("SimpleNode");

let nibKey = (connectionNib: connectionNib) => {
  switch (connectionNib) {
  | ValueConnection => "value"
  | NibConnection(nibID) => nibID
  | PositionalConnection(int) => string_of_int(int)
  };
};

let renderNibs =
    (
      nibs: list(displayNib),
      className: string,
      isSource: bool,
      connectionNode: connectionNode,
      emit: GraphActions.graphAction => unit,
      selectedNib: option(connectionNib),
    ) =>
  ReasonReact.array(
    Array.of_list(
      Belt.List.map(
        nibs,
        ({name, nib}: displayNib) => {
          let nameElement =
            <div className="nib-name"> {ReasonReact.string(name)} </div>;
          <div className key={nibKey(nib)}>
            {isSource ? ReasonReact.null : nameElement}
            <Nib
              isSource
              isHighlighted={Some(nib) == selectedNib}
              connectionSide={node: connectionNode, nib}
              emit
            />
            {isSource ? nameElement : ReasonReact.null}
          </div>;
        },
      ),
    ),
  );

let make =
    (
      ~nodeID: nodeID,
      ~definitionID: option(definitionID)=?,
      ~name: option(string)=?,
      ~inputs: list(displayNib),
      ~outputs: list(displayNib),
      ~position: point,
      ~depth: int,
      ~size: point,
      ~selectedNib: option(connectionNib)=?,
      ~emit: GraphActions.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div
      className="node"
      style={ReactDOMRe.Style.make(
        ~left=pixels(position.x),
        ~top=pixels(position.y),
        ~width=pixels(size.x),
        ~height=pixels(size.y),
        ~zIndex=string_of_int(depth),
        (),
      )}
      onDoubleClick=?{
        Belt.Option.map(definitionID, (definitionID: definitionID, _event) =>
          ReasonReact.Router.push("#" ++ definitionID)
        )
      }>
      {switch (name) {
       | Some(name) =>
         <div className="name"> {ReasonReact.string(name)} </div>
       | None => ReasonReact.null
       }}
      {renderNibs(
         inputs,
         "input",
         false,
         NodeConnection(nodeID),
         emit,
         selectedNib,
       )}
      {renderNibs(
         outputs,
         "output",
         true,
         NodeConnection(nodeID),
         emit,
         selectedNib,
       )}
    </div>,
};

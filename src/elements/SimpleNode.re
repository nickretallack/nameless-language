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
      nodeID: nodeID,
      emit: GraphActions.graphAction => unit,
      selectedNib: option(connectionNib),
    ) =>
  ReasonReact.array(
    Array.of_list(
      Belt.List.map(nibs, ({name, nib}) =>
        <div className key={nibKey(nib)}>
          {isSource ? ReasonReact.null : ReasonReact.string(name)}
          <Nib
            isSource
            isHighlighted={Some(nib) == selectedNib}
            connectionSide={node: NodeConnection(nodeID), nib}
            emit
          />
          {isSource ? ReasonReact.string(name) : ReasonReact.null}
        </div>
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
      ~selectedNib: option(connectionNib)=?,
      ~emit: GraphActions.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div
      className="node"
      style={positionStyle(position)}
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
      {renderNibs(inputs, "input", false, nodeID, emit, selectedNib)}
      {renderNibs(outputs, "output", true, nodeID, emit, selectedNib)}
    </div>,
};
open Definition;
open Helpers;

let component = ReasonReact.statelessComponent("Node");

let make =
    (
      ~nodeID: nodeID,
      ~definition: definition,
      ~position: point,
      ~emit: ActionTypes.graphAction => unit,
      _children,
    ) => {
  ...component,
  render: _self =>
    <div className="node" style={positionStyle(position)}>
      {ReasonReact.string("todo")}
    </div>,
};

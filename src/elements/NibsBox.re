open Helpers;

let component = ReasonReact.statelessComponent("NibNames");
let make =
    (
      ~position: point,
      ~height: float,
      ~nodeWidth: float,
      ~textHeight: float,
      _children,
    ) => {
  ...component,
  render: _self => {
    <rect
      x={pixels(position.x)}
      y={pixels(position.y)}
      width={pixels(nodeWidth)}
      height={pixels(height *. textHeight)}
      fill="black"
      fillOpacity="0.1"
    />;
  },
};

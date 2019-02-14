[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("NibNames");
let make =
    (
      ~name=?,
      ~position: point,
      ~height: float,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~onDoubleClick=?,
      ~onClick=?,
      ~onMouseDown=?,
      ~onTouchStart=?,
      ~onMouseUp=?,
      ~onTouchEnd=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    <g
      ?onClick
      ?onDoubleClick
      ?onMouseDown
      ?onMouseUp
      ?onTouchStart
      ?onTouchEnd>
      <rect
        x={pixels(position.x)}
        y={pixels(position.y)}
        width={pixels(nodeWidth)}
        height={pixels(height)}
        fill={selected ? "blue" : "black"}
        fillOpacity="0.1"
      />
      {switch (name) {
       | None => ReasonReact.null
       | Some(text) =>
         <text
           textAnchor="middle"
           alignmentBaseline="central"
           x={pixels(position.x +. nodeWidth /. 2.0)}
           y={pixels(position.y +. textHeight /. 2.0)}>
           {ReasonReact.string(text)}
         </text>
       }}
    </g>;
  },
};

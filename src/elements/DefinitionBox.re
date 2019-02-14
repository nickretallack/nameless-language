[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("DefinitionBox");

let make =
    (
      ~name: string,
      ~position: point,
      ~size: point,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~onClick=?,
      ~onDoubleClick=?,
      ~onMouseDown=?,
      ~onTouchStart=?,
      ~onMouseUp=?,
      ~onTouchEnd=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    <g
      ?onDoubleClick
      ?onClick
      ?onMouseDown
      ?onMouseUp
      ?onTouchStart
      ?onTouchEnd>
      <rect
        x={pixels(position.x +. nodeWidth)}
        y={pixels(position.y +. textHeight)}
        width={pixels(size.x -. nodeWidth *. 2.0)}
        height={pixels(size.y -. textHeight)}
        fill={selected ? "blue" : "black"}
        fillOpacity="0.05"
      />
      <NibsBox
        position={x: position.x, y: position.y +. textHeight}
        height={size.y -. textHeight}
        nodeWidth
        textHeight
        selected
      />
      <NibsBox
        position={
          x: position.x +. size.x -. nodeWidth,
          y: position.y +. textHeight,
        }
        height={size.y -. textHeight}
        nodeWidth
        textHeight
        selected
      />
      <rect
        x={pixels(position.x)}
        y={pixels(position.y)}
        width={pixels(size.x)}
        height={pixels(textHeight)}
        fill={selected ? "blue" : "black"}
        fillOpacity="0.1"
      />
      <text
        textAnchor="middle"
        alignmentBaseline="central"
        x={pixels(position.x +. size.x /. 2.0)}
        y={pixels(position.y +. textHeight /. 2.0)}>
        {ReasonReact.string(name)}
      </text>
    </g>;
  },
};

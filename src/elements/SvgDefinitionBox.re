[%%debugger.chrome];
open Helpers;

let component = ReasonReact.statelessComponent("SvgDefinitionBox");

let make =
    (
      ~name: string,
      ~position: point,
      ~size: point,
      ~nodeWidth: float,
      ~textHeight: float,
      ~onDoubleClick=?,
      _children,
    ) => {
  ...component,
  render: _self => {
    <g ?onDoubleClick>
      <rect
        x={pixels(position.x +. nodeWidth)}
        y={pixels(position.y +. textHeight)}
        width={pixels(size.x -. nodeWidth *. 2.0)}
        height={pixels(size.y -. textHeight)}
        fill="black"
        fillOpacity="0.05"
      />
      <NibsBox
        position={x: position.x, y: position.y +. textHeight}
        height={size.y -. textHeight}
        nodeWidth
        textHeight
      />
      <NibsBox
        position={
          x: position.x +. size.x -. nodeWidth,
          y: position.y +. textHeight,
        }
        height={size.y -. textHeight}
        nodeWidth
        textHeight
      />
      <rect
        x={pixels(position.x)}
        y={pixels(position.y)}
        width={pixels(size.x)}
        height={pixels(textHeight)}
        fill="black"
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

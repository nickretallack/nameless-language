[@react.component]
let make =
    (
      ~name=?,
      ~position: Point.t,
      ~height: float,
      ~nodeWidth: float,
      ~textHeight: float,
      ~selected: bool=false,
      ~onDoubleClick=?,
      ~onClick=?,
      ~onPointerDown=?,
      ~onPointerUp=?,
    ) => {
  <g ?onClick ?onDoubleClick ?onPointerDown ?onPointerUp>
    <rect
      x={FloatToPixels.f(position.x)}
      y={FloatToPixels.f(position.y)}
      width={FloatToPixels.f(nodeWidth)}
      height={FloatToPixels.f(height)}
      fill={selected ? "blue" : "black"}
      fillOpacity="0.1"
    />
    {switch (name) {
     | None => ReasonReact.null
     | Some(text) =>
       <text
         textAnchor="middle"
         alignmentBaseline="central"
         x={FloatToPixels.f(position.x +. nodeWidth /. 2.0)}
         y={FloatToPixels.f(position.y +. textHeight /. 2.0)}>
         {ReasonReact.string(text)}
       </text>
     }}
  </g>;
};

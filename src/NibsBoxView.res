@react.component
let make = (
  ~name=?,
  ~nodeID: option<NodeID.t>=?,
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
  let box = React.useRef(Js.Nullable.null)
  let id = switch nodeID {
  | Some(value) => value
  | None => ""
  }
  React.useEffect(() => {
    RefDisableScrollingWhileDragging.f(box)
    Some(() => RefDisableScrollingWhileDragging.undo(box))
  })
  <g id ref={ReactDOM.Ref.domRef(box)} ?onClick ?onDoubleClick ?onPointerDown ?onPointerUp>
    <rect
      x={FloatToPixels.f(position.x)}
      y={FloatToPixels.f(position.y)}
      width={FloatToPixels.f(nodeWidth)}
      height={FloatToPixels.f(height)}
      fill={selected ? "blue" : "black"}
      fillOpacity="0.1"
    />
    {switch name {
    | None => React.null
    | Some(text) =>
      <text
        textAnchor="middle"
        alignmentBaseline="central"
        x={FloatToPixels.f(position.x +. nodeWidth /. 2.0)}
        y={FloatToPixels.f(position.y +. textHeight /. 2.0)}>
        {React.string(text)}
      </text>
    }}
  </g>
}

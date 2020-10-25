[@react.component]
let make =
    (
      ~name: string,
      ~position: Point.t,
      ~size: Point.t,
      ~nodeWidth: float,
      ~textHeight: float,
      ~nodeScope: NodeScope.t,
      ~emit,
      ~selected: bool=false,
      ~onClick=?,
      ~onDoubleClick=?,
      ~onPointerDown=?,
      ~onPointerUp=?,
    ) => {
  <g ?onDoubleClick ?onClick ?onPointerDown ?onPointerUp>
    <rect
      x={FloatToPixels.f(position.x +. nodeWidth)}
      y={FloatToPixels.f(position.y +. textHeight)}
      width={FloatToPixels.f(size.x -. nodeWidth *. 2.0)}
      height={FloatToPixels.f(size.y -. textHeight)}
      fill={selected ? "blue" : "black"}
      fillOpacity="0.05"
      ref={ReactDOMRe.Ref.callbackDomRef(nullableElement =>
        switch (Js.Nullable.toOption(nullableElement)) {
        | None => ()
        | Some(element) =>
          Webapi.Dom.Element.addEventListener(
            "finish-dragging",
            event => {
              emit(
                GraphAction.PointerAction({
                  pointerID: EventGetDetail.f(event)##identifier,
                  action: FinishDragging(nodeScope),
                }),
              )
            },
            element,
          )
        }
      )}
    />
    <NibsBoxView
      position={x: position.x, y: position.y +. textHeight}
      height={size.y -. textHeight}
      nodeWidth
      textHeight
      selected
    />
    <NibsBoxView
      position={
        x: position.x +. size.x -. nodeWidth,
        y: position.y +. textHeight,
      }
      height={size.y -. textHeight}
      nodeWidth
      textHeight
      selected
    />
    <g ref=RefDisableScrollingWhileDragging.f>
      <rect
        x={FloatToPixels.f(position.x)}
        y={FloatToPixels.f(position.y)}
        width={FloatToPixels.f(size.x)}
        height={FloatToPixels.f(textHeight)}
        fill={selected ? "blue" : "black"}
        fillOpacity="0.1"
      />
      <text
        textAnchor="middle"
        alignmentBaseline="central"
        x={FloatToPixels.f(position.x +. size.x /. 2.0)}
        y={FloatToPixels.f(position.y +. textHeight /. 2.0)}>
        {ReasonReact.string(name)}
      </text>
    </g>
  </g>;
};

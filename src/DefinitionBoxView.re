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
  let body = React.useRef(Js.Nullable.null);
  let title = React.useRef(Js.Nullable.null);
  React.useEffect(() => {
    switch (Js.Nullable.toOption(title.current)) {
    | None => ()
    | Some(element) => DisableScrollingWhileDragging.f(element)
    };

    let finishDraggingHandler = event => {
      emit(
        GraphAction.PointerAction({
          pointerID: EventGetDetail.f(event)##identifier,
          action: FinishDragging(nodeScope),
        }),
      );
    };

    switch (Js.Nullable.toOption(body.current)) {
    | None => ()
    | Some(element) =>
      Webapi.Dom.Element.addEventListener(
        "finish-dragging",
        finishDraggingHandler,
        element,
      )
    };

    Some(
      () => {
        switch (Js.Nullable.toOption(title.current)) {
        | None => ()
        | Some(element) => DisableScrollingWhileDragging.undo(element)
        };

        switch (Js.Nullable.toOption(body.current)) {
        | None => ()
        | Some(element) =>
          Webapi.Dom.Element.removeEventListener(
            "finish-dragging",
            finishDraggingHandler,
            element,
          )
        };
      },
    );
  });

  <g ?onDoubleClick ?onClick ?onPointerDown ?onPointerUp>
    <rect
      x={FloatToPixels.f(position.x +. nodeWidth)}
      y={FloatToPixels.f(position.y +. textHeight)}
      width={FloatToPixels.f(size.x -. nodeWidth *. 2.0)}
      height={FloatToPixels.f(size.y -. textHeight)}
      fill={selected ? "blue" : "black"}
      fillOpacity="0.05"
      ref={ReactDOM.Ref.domRef(body)}
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
    <g ref={ReactDOM.Ref.domRef(title)}>
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

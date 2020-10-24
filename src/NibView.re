[@react.component]
let make =
    (
      ~isSource: bool,
      ~connectionSide: ConnectionSide.t,
      ~position: Point.t,
      ~text: string,
      ~isHighlighted: bool,
      ~definitions: DefinitionMap.t,
      ~languageName: LanguageName.t,
      ~value: option(Value.t),
      ~emit: GraphAction.t => unit,
    ) => {
  let sidePadding = 10.0;
  let color = "rgb(160,160,160)";

  <>
    <text
      textAnchor={isSource ? "start" : "end"}
      alignmentBaseline="central"
      x={FloatToPixels.f(
        position.x +. sidePadding *. (isSource ? 1.0 : (-1.0)),
      )}
      y={FloatToPixels.f(position.y)}>
      {ReasonReact.string(text)}
    </text>
    {switch (value) {
     | None => ReasonReact.null
     | Some(value) =>
       <text
         textAnchor="end"
         x={FloatToPixels.f(position.x -. sidePadding)}
         y={FloatToPixels.f(position.y)}
         alignmentBaseline="central">
         {ReasonReact.string(
            ValueDisplay.f(value, definitions, languageName),
          )}
       </text>
     }}
    <circle
      cx={FloatToPixels.f(position.x)}
      cy={FloatToPixels.f(position.y)}
      r="6"
      stroke={isHighlighted ? "red" : color}
      strokeWidth="2"
      fill={isSource ? color : "white"}
    />
    <rect
      width="20"
      height="20"
      fill="transparent"
      x={FloatToPixels.f(position.x -. 10.0)}
      y={FloatToPixels.f(position.y -. 10.0)}
      ref={ReactDOMRe.Ref.callbackDomRef(nullableElement =>
        switch (Js.Nullable.toOption(nullableElement)) {
        | None => ()
        | Some(element) =>
          Webapi.Dom.Element.addEventListener(
            "finish-drawing",
            event =>
              emit(
                PointerAction({
                  pointerID: EventGetDetail.f(event)##identifier,
                  action: FinishDrawing({connectionSide, isSource}),
                }),
              ),
            element,
          );
          // Prevent scrolling while we're drawing.  Must be non-passive.
          Webapi.Dom.Element.addEventListenerWithOptions(
            "touchstart",
            Webapi.Dom.Event.preventDefault,
            {"passive": false, "capture": false, "once": false},
            element,
          );
        }
      )}
      onLostPointerCapture={_ => Js.log("LOST")}
      onGotPointerCapture={_ => Js.log("GOT")}
      onPointerDown={event => {
        let pointerID = ReactEvent.Pointer.pointerId(event);
        let _ =
          ReactEvent.Pointer.target(event)##setPointerCapture(pointerID);
        emit(
          PointerAction({
            pointerID,
            action:
              StartDrawing({
                explicitConnectionSide: {
                  connectionSide,
                  isSource,
                },
                point: PointFromPointerEvent.f(event),
              }),
          }),
        );
      }}
      onPointerUp={event => {
        // Fire an event on the nib we're connecting to.
        let _ =
          Webapi.Dom.Element.dispatchEvent(
            Webapi.Dom.CustomEvent.makeWithOptions(
              "finish-drawing",
              {
                "detail": {
                  "identifier": ReactEvent.Pointer.pointerId(event),
                },
              },
            ),
            Webapi.Dom.Document.elementFromPoint(
              ReactEvent.Pointer.clientX(event),
              ReactEvent.Pointer.clientY(event),
              Webapi.Dom.document,
            ),
          );
        ();
        emit(
          PointerAction({
            pointerID: ReactEvent.Pointer.pointerId(event),
            action: ReleasePointer,
          }),
        );
      }}
      onPointerMove={event => {
        ReactEvent.Pointer.preventDefault(event);
        ReactEvent.Pointer.stopPropagation(event);
        emit(
          PointerAction({
            pointerID: ReactEvent.Pointer.pointerId(event),
            action: MovePointer(PointFromPointerEvent.f(event)),
          }),
        );
      }}
    />
  </>;
};

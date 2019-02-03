[%%debugger.chrome];
open Definition;
open GraphActions;
open Helpers;

[@bs.get] external getEventDetail: Dom.event => Js.t({..}) = "detail";

let sidePadding = 10.0;
let component = ReasonReact.reducerComponent("Nib");
let make =
    (
      ~isSource: bool,
      ~connectionSide: connectionSide,
      ~position: point,
      ~text: string,
      ~emit: graphAction => unit,
      ~isHighlighted: bool,
      _children,
    ) => {
  ...component,
  initialState: () => ref(None),
  reducer: (_: unit, _) => ReasonReact.NoUpdate,
  didMount: self =>
    switch (self.state^) {
    | Some(element) =>
      Webapi.Dom.Element.addEventListener(
        "finish-drawing",
        event =>
          emit({
            pointerID: Touch(getEventDetail(event)##identifier),
            action: FinishDrawing({connectionSide, isSource}),
          }),
        element,
      )
    | None => ()
    },
  render: self =>
    <>
      <text
        textAnchor={isSource ? "start" : "end"}
        alignmentBaseline="central"
        x={pixels(position.x +. sidePadding *. (isSource ? 1.0 : (-1.0)))}
        y={pixels(position.y)}>
        {ReasonReact.string(text)}
      </text>
      <circle
        ref={ref => self.state := Js.Nullable.toOption(ref)}
        cx={pixels(position.x)}
        cy={pixels(position.y)}
        r="6"
        stroke="lightgray"
        strokeWidth="4"
        fill={isSource ? "lightgray" : "white"}
        onMouseDown={event =>
          emit({
            pointerID: Mouse,
            action:
              StartDrawing({
                explicitConnectionSide: {
                  connectionSide,
                  isSource,
                },
                point: pointFromMouse(event),
              }),
          })
        }
        onTouchStart={event =>
          iterateTouches(event, touch =>
            emit({
              pointerID: Touch(touch##identifier),
              action:
                StartDrawing({
                  explicitConnectionSide: {
                    connectionSide,
                    isSource,
                  },
                  point: {
                    x: touch##clientX,
                    y: touch##clientY,
                  },
                }),
            })
          )
        }
        onMouseUp={_ =>
          emit({
            pointerID: Mouse,
            action: FinishDrawing({connectionSide, isSource}),
          })
        }
        onTouchEnd={event =>
          iterateTouches(event, touch =>
            Webapi.Dom.Element.dispatchEvent(
              Webapi.Dom.CustomEvent.makeWithOptions(
                "finish-drawing",
                {
                  "detail": {
                    "identifier": touch##identifier,
                  },
                },
              ),
              Webapi.Dom.Document.elementFromPoint(
                touch##clientX,
                touch##clientY,
                Webapi.Dom.document,
              ),
            )
            |> ignore
          )
        }
      />
    </>,
};

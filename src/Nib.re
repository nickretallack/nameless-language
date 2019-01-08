[%%debugger.chrome];
open Definition;
open GraphActions;
open Helpers;

[@bs.get] external getEventDetail: Dom.event => Js.t({..}) = "detail";

let component = ReasonReact.reducerComponent("Nib");
let make =
    (
      ~isSource: bool,
      ~connectionSide: connectionSide,
      ~emit: graphAction => unit,
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
          emit(
            FinishDrawing({
              pointerID: Touch(getEventDetail(event)##identifier),
              connectionSide,
              isSource,
            }),
          ),
        element,
      )
    | None => ()
    },
  render: self =>
    <div
      ref={ref => self.state := Js.Nullable.toOption(ref)}
      className={String.concat(" ", [isSource ? "source" : "sink", "nib"])}
      onMouseDown={event =>
        emit(
          StartDrawing({
            pointerID: Mouse,
            drawingConnection: {
              connectionSide,
              point: pointFromMouse(event),
              startIsSource: isSource,
            },
          }),
        )
      }
      onTouchStart={event =>
        iterateTouches(event, touch =>
          emit(
            StartDrawing({
              pointerID: Touch(touch##identifier),
              drawingConnection: {
                connectionSide,
                point: {
                  x: touch##clientX,
                  y: touch##clientY,
                },
                startIsSource: isSource,
              },
            }),
          )
        )
      }
      onMouseUp={_ =>
        emit(FinishDrawing({pointerID: Mouse, connectionSide, isSource}))
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
    />,
};

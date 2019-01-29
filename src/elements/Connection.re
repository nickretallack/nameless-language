open Helpers;

let component = ReasonReact.statelessComponent("Connection");

let make =
    (
      ~sourcePosition: point,
      ~sinkPosition: point,
      ~nudge=0,
      ~maxNudge=1,
      _children,
    ) => {
  ...component,
  render: _self => {
    let strokeWidth = 5.0;
    let nudgeMagnitude = 5.0;
    let left = min(sourcePosition.x, sinkPosition.x);
    let top = min(sourcePosition.y, sinkPosition.y) -. strokeWidth /. 2.0;
    let width = abs_float(sourcePosition.x -. sinkPosition.x);
    let height = abs_float(sourcePosition.y -. sinkPosition.y) +. strokeWidth;
    let palette = [|"fuchsia", "aqua", "lime", "blue"|];

    let containerStyle =
      ReactDOMRe.Style.make(
        ~width=pixels(width),
        ~height=pixels(height),
        ~left=pixels(left),
        ~top=pixels(top),
        ~pointerEvents="none",
        ~position="absolute",
        (),
      );

    let sharedStyle =
      ReactDOMRe.Style.make(
        ~height=pixels(height /. 2.0),
        ~width=pixels(width /. 2.0),
        ~position="absolute",
        ~borderStyle="solid",
        ~borderColor=palette[nudge mod Array.length(palette)],
        ~borderWidth="0",
        ~opacity="0.7",
        (),
      );

    let centeredNudge =
      float_of_int(nudge) -. float_of_int(maxNudge - 1) /. 2.0;
    let clampedNudgeMagnitude =
      float_of_int(maxNudge) *. nudgeMagnitude < width ?
        nudgeMagnitude : width /. float_of_int(maxNudge);
    let actualNudge = centeredNudge *. clampedNudgeMagnitude;
    let roundness = 8.0;
    let actualRoundness =
      roundness *. 2.0 < height ? roundness : height /. 2.0;

    let sourceIsHigher = sourcePosition.y > sinkPosition.y;
    let (leftStyle, rightStyle) =
      if (sourcePosition.x < sinkPosition.x ? sourceIsHigher : !sourceIsHigher) {
        let leftWidth =
          width
          /. 2.0
          -. actualNudge
          -. strokeWidth
          -. clampedNudgeMagnitude
          /. 4.0;
        (
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderBottomWidth=pixels(strokeWidth),
              ~borderTopWidth="0",
              ~width=pixels(leftWidth),
              ~borderBottomRightRadius=pixels(actualRoundness),
              ~bottom="0",
              (),
            ),
          ),
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderTopWidth=pixels(strokeWidth),
              ~borderBottomWidth="0",
              ~width=pixels(width -. leftWidth +. strokeWidth),
              ~borderTopLeftRadius=pixels(actualRoundness),
              ~top="0",
              (),
            ),
          ),
        );
      } else {
        let leftWidth = width /. 2.0 +. actualNudge;
        (
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderTopWidth=pixels(strokeWidth),
              ~borderBottomWidth="0",
              ~width=pixels(leftWidth),
              ~borderTopRightRadius=pixels(actualRoundness),
              ~top="0",
              (),
            ),
          ),
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderBottomWidth=pixels(strokeWidth),
              ~borderTopWidth="0",
              ~width=pixels(width -. leftWidth +. strokeWidth),
              ~borderBottomLeftRadius=pixels(actualRoundness),
              ~bottom="0",
              (),
            ),
          ),
        );
      };

    let fullLeftStyle =
      ReactDOMRe.Style.combine(
        leftStyle,
        ReactDOMRe.Style.make(
          ~left="0",
          ~borderRightWidth=pixels(strokeWidth),
          (),
        ),
      );

    let fullRightStyle =
      ReactDOMRe.Style.combine(
        rightStyle,
        ReactDOMRe.Style.make(
          ~right="0",
          ~borderLeftWidth=pixels(strokeWidth),
          (),
        ),
      );

    <div style=containerStyle>
      <div style=fullLeftStyle />
      <div style=fullRightStyle />
    </div>;
  },
};

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
    let strokeWidth = 5;
    let nudgeMagnitude = 20;
    let left = min(sourcePosition.x, sinkPosition.x);
    let top = min(sourcePosition.y, sinkPosition.y) - strokeWidth / 2;
    let width = abs(sourcePosition.x - sinkPosition.x);
    let height = abs(sourcePosition.y - sinkPosition.y) + strokeWidth;
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
        ~height=floatPixels(float_of_int(height) /. 2.0),
        ~width=floatPixels(float_of_int(width) /. 2.0),
        ~position="absolute",
        ~borderStyle="solid",
        ~borderColor=palette[nudge mod Array.length(palette)],
        ~borderWidth="0",
        (),
      );

    let centeredNudge = nudge - (maxNudge - 1) / 2;
    let clampedNudgeMagnitude =
      maxNudge * nudgeMagnitude < width ? nudgeMagnitude : width / maxNudge;
    let actualNudge = centeredNudge * clampedNudgeMagnitude;
    let roundness = 8;
    let actualRoundness = roundness * 2 < height ? roundness : height / 2;

    let sourceIsHigher = sourcePosition.y > sinkPosition.y;
    let (leftStyle, rightStyle) =
      if (sourcePosition.x < sinkPosition.x ? sourceIsHigher : !sourceIsHigher) {
        (
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderBottomWidth=pixels(strokeWidth),
              ~borderTopWidth="0",
              ~right=
                floatPixels(
                  float_of_int(width)
                  /. 2.0
                  -. float_of_int(actualNudge)
                  -. float_of_int(strokeWidth)
                  -. float_of_int(clampedNudgeMagnitude)
                  /. 4.0,
                ),
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
              ~left=
                floatPixels(
                  float_of_int(width)
                  /. 2.0
                  +. float_of_int(actualNudge)
                  +. float_of_int(clampedNudgeMagnitude)
                  /. 4.0,
                ),
              ~borderTopLeftRadius=pixels(actualRoundness),
              ~top="0",
              (),
            ),
          ),
        );
      } else {
        (
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~borderTopWidth=pixels(strokeWidth),
              ~borderBottomWidth="0",
              ~right=
                floatPixels(
                  float_of_int(width) /. 2.0 +. float_of_int(actualNudge),
                ),
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
              ~left=
                floatPixels(
                  float_of_int(width)
                  /. 2.0
                  -. float_of_int(actualNudge)
                  -. float_of_int(strokeWidth),
                ),
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

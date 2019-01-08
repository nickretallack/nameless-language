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
        ~height=pixels(height / 2),
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
              ~left="0",
              ~borderRightWidth=pixels(strokeWidth),
              ~borderBottomWidth=pixels(strokeWidth),
              ~borderTopWidth="0",
              ~right=
                pixels(
                  width
                  / 2
                  - actualNudge
                  - strokeWidth
                  - clampedNudgeMagnitude
                  / 4,
                ),
              ~borderBottomRightRadius=pixels(actualRoundness),
              ~bottom="0",
              (),
            ),
          ),
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~right="0",
              ~borderLeftWidth=pixels(strokeWidth),
              ~borderTopWidth=pixels(strokeWidth),
              ~borderBottomWidth="0",
              ~left=
                pixels(width / 2 + actualNudge + clampedNudgeMagnitude / 4),
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
              ~left="0",
              ~borderRightWidth=pixels(strokeWidth),
              ~borderTopWidth=pixels(strokeWidth),
              ~borderBottomWidth="0",
              ~right=pixels(width / 2 + actualNudge),
              ~borderTopRightRadius=pixels(actualRoundness),
              ~top="0",
              (),
            ),
          ),
          ReactDOMRe.Style.combine(
            sharedStyle,
            ReactDOMRe.Style.make(
              ~right="0",
              ~borderLeftWidth=pixels(strokeWidth),
              ~borderBottomWidth=pixels(strokeWidth),
              ~borderTopWidth="0",
              ~left=pixels(width / 2 - actualNudge - strokeWidth),
              ~borderBottomLeftRadius=pixels(actualRoundness),
              ~bottom="0",
              (),
            ),
          ),
        );
      };

    <div style=containerStyle>
      <div style=leftStyle />
      <div style=rightStyle />
    </div>;
  },
};

[%bs.raw {|require('./Pull.css')|}];

module SpringHook =
  Spring.MakeSpring({
    type t = (float, float);
    type interpolate = (float, float) => string;
  });

let clamp = (num, lower, upper) =>
  if (num < lower) {
    lower;
  } else if (num > upper) {
    upper;
  } else {
    num;
  };

[@react.component]
let make = () => {
  let (vals, set) = SpringHook.use((0., 0.));

  let gestureProps =
    Gesture.useGesture(.
      props => {
        let down = props->Gesture.downGet;
        let delta = props->Gesture.deltaGet;
        let velocity = props->Gesture.velocityGet;
        let vel = clamp(velocity, 1., 8.);
        let xy = down ? delta : (0., 0.);
        set(
          ~config=
            Spring.config(~mass=vel, ~tension=500. *. vel, ~friction=50.),
          xy,
        );
      },
      (),
    );
  <Spring.Div
    spreadProps=gestureProps
    style={ReactDOMRe.Style.make(
      ~transform=
        vals->SpringHook.interpolate((x, y) =>
          {j|translate3d($(x)px,$(y)px,0)|j}
        ),
      (),
    )}
  />;
};
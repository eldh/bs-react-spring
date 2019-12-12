[%bs.raw {|require('./ViewPager.css')|}];

let pages = [|
  "https://images.pexels.com/photos/62689/pexels-photo-62689.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260",
  "https://images.pexels.com/photos/296878/pexels-photo-296878.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260",
  "https://images.pexels.com/photos/1509428/pexels-photo-1509428.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260",
  "https://images.pexels.com/photos/351265/pexels-photo-351265.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260",
  "https://images.pexels.com/photos/924675/pexels-photo-924675.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260",
|];
let clamp = (num, lower, upper) =>
  if (num < lower) {
    lower;
  } else if (num > upper) {
    upper;
  } else {
    num;
  };
type springData = {
  .
  "x": float,
  "sc": int,
  "display": string,
};
module Springs =
  Spring.MakeSprings({
    type t = springData;
    type interpolate = int => string;
  });
let number = pages->Array.length;

[@react.component]
let make = () => {
  let (windowWidth, _) = Window.useWindowSize();
  let windowWidthF = windowWidth->float_of_int;
  let index = React.useRef(0);
  let (props, set) =
    Springs.use(~number, i =>
      {"x": i->float_of_int *. windowWidthF, "sc": 1, "display": "block"}
    );
  let spreadProps =
    Gesture.useGesture(.
      prps => {
        let down = prps.down;
        let (xDelta, _yDelta) = prps.delta;
        let (xDir, _ydir) = prps.direction;
        let cancel = prps.cancel;
        let distance = prps.distance;

        let sc =
          down ? (1. -. distance /. windowWidthF /. 2.)->int_of_float : 1;
        if (down && distance > windowWidthF /. 2.) {
          cancel(.
            {
              let newVal =
                clamp(
                  React.Ref.current(index)
                  + (xDir->int_of_float > 0 ? (-1) : 1),
                  0,
                  number - 1,
                );
              React.Ref.setCurrent(index, newVal);
              newVal;
            },
          );
        };
        set(i => {
          let x =
            (i - React.Ref.current(index))
            * windowWidth
            + (down ? xDelta->int_of_float : 0);
          let rv = {"x": x->float_of_int, "sc": sc, "display": "block"};
          Js.log2("rv", rv);
          rv;
        });
      },
      (),
    );
  Js.log4("props, set", props, set, index);
  <div className="pager">
    {props
     ->Belt.Array.mapWithIndex((i, p) => {
         let url = pages[i];
         Js.log2("p", p);

         <Spring.Div
           key={i->string_of_int} spreadProps style={ReactDOMRe.Style.make()}>
           <Spring.Div
             style={ReactDOMRe.Style.make(
               ~transform=p##sc->Springs.interpolate(s => {j|scale($(s))|j}),
               ~backgroundImage={j|url($(url))|j},
               (),
             )}
           />
         </Spring.Div>;
       })
     ->React.array}
  </div>;
};
/*
     let bind = useGesture(({ down, delta: [xDelta], direction: [xDir], distance, cancel }) => {
       if (down && distance > window.innerWidth / 2)
         cancel((index.current = clamp(index.current + (xDir > 0 ? -1 : 1), 0, pages.length - 1)))
       set(i => {
         if (i < index.current - 1 && i > index.current + 1) return { display: 'none' }
         let x = (i - index.current) * window.innerWidth + (down ? xDelta : 0)
         let sc = down ? 1 - distance / window.innerWidth / 2 : 1
         return { x, sc, display: 'block' }
       })
     })
     return props.map(({ x, display, sc }, i) => (
       <animated.div {...bind()} key={i} style={{ display, transform: x.interpolate(x => `translate3d(${x}px,0,0)`) }}>
         <animated.div style={{ transform: sc.interpolate(s => `scale(${s})`), backgroundImage: `url(${pages[i]})` }} />
       </animated.div>
     ))
 import { render } from 'react-dom'
 import React, { useRef } from 'react'
 import clamp from 'lodash-es/clamp'
 import { useSprings, animated } from 'react-spring'
 import { useGesture } from 'react-with-gesture'
 import './styles.css'

 let pages = [
   'https://images.pexels.com/photos/62689/pexels-photo-62689.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260',
   'https://images.pexels.com/photos/296878/pexels-photo-296878.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260',
   'https://images.pexels.com/photos/1509428/pexels-photo-1509428.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260',
   'https://images.pexels.com/photos/351265/pexels-photo-351265.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260',
   'https://images.pexels.com/photos/924675/pexels-photo-924675.jpeg?auto=compress&cs=tinysrgb&dpr=2&h=750&w=1260'
 ]

 function Viewpager() {
   let index = useRef(0)
   let [props, set] = useSprings(pages.length, i => ({ x: i * window.innerWidth, sc: 1, display: 'block' }))
   let bind = useGesture(({ down, delta: [xDelta], direction: [xDir], distance, cancel }) => {
     if (down && distance > window.innerWidth / 2)
       cancel((index.current = clamp(index.current + (xDir > 0 ? -1 : 1), 0, pages.length - 1)))
     set(i => {
       if (i < index.current - 1 && i > index.current + 1) return { display: 'none' }
       let x = (i - index.current) * window.innerWidth + (down ? xDelta : 0)
       let sc = down ? 1 - distance / window.innerWidth / 2 : 1
       return { x, sc, display: 'block' }
     })
   })
   return props.map(({ x, display, sc }, i) => (
     <animated.div {...bind()} key={i} style={{ display, transform: x.interpolate(x => `translate3d(${x}px,0,0)`) }}>
       <animated.div style={{ transform: sc.interpolate(s => `scale(${s})`), backgroundImage: `url(${pages[i]})` }} />
     </animated.div>
   ))
 }

 render(<Viewpager />, document.getElementById('root'))
  */
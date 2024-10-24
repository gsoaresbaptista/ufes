import { IconProps } from "./types";

function LotusIcon({ className, width = 40, height = 40 }: IconProps) {
  return (
    <svg width={`${width}px`} height={`${height}px`} viewBox="0 0 40 40">
      <path
        className={className}
        d="M 39.738281 20.136719 C 39.136719 19.480469 38.351562 19.015625 37.480469 18.71875
                 C 36.617188 18.429688 35.652344 18.296875 34.636719 18.296875
                 C 34.082031 18.296875 33.511719 18.332031 32.929688 18.414062
                 C 33.074219 17.578125 33.152344 16.714844 33.152344 15.84375
                 C 33.152344 14.296875 32.90625 12.714844 32.347656 11.136719 L 32.160156 10.609375
                 L 31.597656 10.652344 C 29.234375 10.855469 27.003906 11.640625 25.027344 12.808594
                 C 24.0625 10.71875 21.945312 8.132812 20 7.035156
                 C 18.054688 8.132812 15.9375 10.71875 14.972656 12.808594
                 C 12.996094 11.640625 10.765625 10.855469 8.402344 10.652344
                 L 7.839844 10.609375 L 7.652344 11.136719 C 7.09375 12.714844 6.847656 14.296875 6.847656 15.84375
                 C 6.847656 16.714844 6.925781 17.578125 7.070312 18.414062
                 C 6.492188 18.332031 5.917969 18.296875 5.367188 18.296875
                 C 4.347656 18.296875 3.382812 18.429688 2.519531 18.71875
                 C 1.648438 19.015625 0.863281 19.480469 0.261719 20.136719 L 0 20.417969 L 0.0859375 20.796875
                 C 1.148438 25.398438 3.339844 28.503906 6.25 30.394531
                 C 9.164062 32.289062 12.71875 32.964844 16.53125 32.964844 C 17.542969 32.964844 18.574219 32.914062 19.609375 32.824219
                 C 19.636719 32.832031 19.96875 32.828125 20 32.832031 C 20.027344 32.828125 20.363281 32.832031 20.390625 32.824219
                 C 21.425781 32.914062 22.457031 32.964844 23.46875 32.964844 C 27.28125 32.964844 30.835938 32.289062 33.75 30.394531
                 C 36.660156 28.503906 38.851562 25.398438 39.914062 20.796875 L 40 20.417969 Z M 26.757812 20.433594
                 C 26.683594 18.296875 26.308594 16.132812 25.589844 14.164062 C 27.289062 13.136719 29.175781 12.425781 31.164062 12.167969
                 C 31.535156 13.402344 31.703125 14.632812 31.703125 15.84375 C 31.703125 16.808594 31.59375 17.761719 31.402344 18.691406
                 C 31.289062 19.234375 31.140625 19.773438 30.976562 20.300781 C 30.53125 21.664062 29.914062 22.972656 29.183594 24.164062
                 C 28.070312 25.980469 26.707031 27.539062 25.355469 28.703125 C 26.199219 26.828125 26.671875 24.503906 26.75 22.082031
                 C 26.765625 21.804688 26.773438 21.527344 26.773438 21.253906 C 26.773438 20.984375 26.765625 20.707031 26.757812 20.433594
                 Z M 8.835938 12.167969 C 10.824219 12.425781 12.710938 13.136719 14.410156 14.164062
                 C 13.691406 16.132812 13.316406 18.296875 13.242188 20.433594 C 13.234375 20.707031 13.226562 20.984375 13.226562 21.253906
                 C 13.226562 21.527344 13.234375 21.804688 13.25 22.082031 C 13.328125 24.503906 13.800781 26.828125 14.644531 28.703125
                 C 13.292969 27.539062 11.929688 25.980469 10.816406 24.164062 C 10.085938 22.972656 9.46875 21.664062 9.023438 20.300781
                 C 8.859375 19.773438 8.710938 19.234375 8.597656 18.691406 C 8.40625 17.761719 8.296875 16.808594 8.296875 15.84375
                 C 8.296875 14.632812 8.464844 13.402344 8.835938 12.167969 Z M 7.042969 29.171875
                 C 4.550781 27.546875 2.636719 24.960938 1.597656 20.859375 C 1.976562 20.535156 2.4375 20.28125 2.984375 20.097656
                 C 3.667969 19.867188 4.472656 19.75 5.367188 19.75 C 6.011719 19.75 6.703125 19.808594 7.414062 19.9375
                 C 8.167969 22.761719 9.632812 25.3125 11.3125 27.359375 C 12.554688 28.863281 13.902344 30.097656 15.226562 30.96875
                 C 15.53125 31.171875 15.835938 31.351562 16.132812 31.503906 C 12.648438 31.460938 9.527344 30.800781 7.042969 29.171875
                 Z M 22.210938 30.640625 C 22.191406 30.65625 22.167969 30.675781 22.148438 30.691406 C 21.695312 31.03125 21.21875 31.25 20.695312 31.34375
                 C 20.660156 31.351562 20.628906 31.351562 20.59375 31.359375 L 20.5625 31.359375 C 20.535156 31.367188 20.5 31.367188 20.460938 31.375
                 C 20.460938 31.375 20.460938 31.375 20.453125 31.375 C 20.40625 31.378906 20.050781 31.378906 20 31.382812
                 C 19.949219 31.378906 19.59375 31.378906 19.546875 31.375 C 19.539062 31.375 19.539062 31.375 19.539062 31.375
                 C 19.5 31.367188 19.464844 31.367188 19.4375 31.359375 L 19.40625 31.359375 C 19.371094 31.351562 19.34375 31.351562 19.304688 31.34375
                 C 18.78125 31.25 18.304688 31.03125 17.851562 30.691406 C 17.832031 30.675781 17.808594 30.65625 17.789062 30.640625
                 C 16.859375 29.90625 16.058594 28.601562 15.515625 26.945312 C 15.128906 25.78125 14.875 24.453125 14.761719 23.054688
                 C 14.710938 22.464844 14.679688 21.871094 14.679688 21.261719 L 14.679688 21.253906 C 14.679688 19.132812 15 16.953125 15.652344 15
                 C 15.761719 14.675781 15.878906 14.363281 16.003906 14.058594 C 16.066406 13.90625 16.132812 13.753906 16.207031 13.601562
                 C 17.011719 11.792969 18.519531 9.851562 20 8.902344 C 21.480469 9.851562 22.988281 11.792969 23.792969 13.601562
                 C 23.867188 13.753906 23.933594 13.90625 23.996094 14.058594 C 24.121094 14.363281 24.238281 14.675781 24.347656 15
                 C 25 16.953125 25.320312 19.132812 25.320312 21.253906 L 25.320312 21.261719 C 25.320312 21.871094 25.289062 22.464844 25.238281 23.054688
                 C 25.125 24.453125 24.871094 25.78125 24.484375 26.945312 C 23.941406 28.601562 23.140625 29.90625 22.210938 30.640625 Z M 32.957031 29.171875
                 C 30.472656 30.800781 27.351562 31.460938 23.867188 31.503906 C 24.164062 31.351562 24.46875 31.171875 24.773438 30.96875
                 C 26.097656 30.097656 27.445312 28.863281 28.6875 27.359375 C 30.367188 25.3125 31.832031 22.761719 32.585938 19.9375
                 C 33.296875 19.808594 33.988281 19.75 34.636719 19.75 C 35.527344 19.75 36.332031 19.867188 37.015625 20.097656
                 C 37.5625 20.277344 38.023438 20.535156 38.402344 20.859375 C 37.363281 24.960938 35.449219 27.546875 32.957031 29.171875
                 Z M 32.957031 29.171875 "
      />
    </svg>
  );
}

export default LotusIcon;

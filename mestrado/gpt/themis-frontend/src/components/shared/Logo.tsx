import LotusIcon from "../icons/LotusIcon";

function Logo({ bigVersion }: { bigVersion: boolean }) {
  const fontSize = bigVersion ? "text-3xl" : "text-lg";

  return (
    <div className="flex items-center gap-2">
      <div className="fill-primary">
        <LotusIcon />
      </div>
      <h2
        className={`text-primary font-space-grotesk font-bold ${fontSize}`}
      >
        Themis
      </h2>
    </div>
  );
}

export default Logo;

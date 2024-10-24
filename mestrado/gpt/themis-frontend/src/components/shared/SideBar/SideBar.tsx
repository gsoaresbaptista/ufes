import { Link } from "react-router-dom";
import Logo from "../Logo";
import UserAvatar from "../UserAvatar";
import SideBarMenu from "./SideBarMenu";

function SideBar() {
  return (
    <nav className="flex flex-col flex-1 sidebar h-screen max-w-[280px]">
      <div className="flex flex-col flex-1 gap-11">
        <Link to="/" className="block">
          <div className=""><Logo bigVersion={true} /></div>
        </Link>
        <UserAvatar />
        <SideBarMenu />
      </div>
    </nav>
  )
}

export default SideBar;

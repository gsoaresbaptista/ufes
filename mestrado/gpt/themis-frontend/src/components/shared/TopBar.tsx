import { Link } from "react-router-dom";
import Logo from "./Logo";
import { Button } from "../ui/button";
import LogoutDialog from "./LogoutDialog";
import { useState } from "react";
import UserAvatar from "./UserAvatar";
import LogoutIcon from "../icons/logoutIcon";


function TopBar() {
  const [openDialog, setOpenDialog] = useState(false);


  return (
    <section className="topbar">
      <div className="flex-between py-4 px-5 flex justify-between">
        <Link to="/">
          <Logo bigVersion={false} />
        </Link>
        <div className="flex gap-2">
          <LogoutDialog open={openDialog} setOpen={setOpenDialog} />
          <UserAvatar />
          <Button
            variant="ghost"
            className="shad-button_ghost"
            onClick={() => {
              setOpenDialog(!openDialog);
            }}
          >
            <LogoutIcon className="dark:fill-primary fill-black" />
          </Button>
        </div>
      </div>
    </section>
  );
}

export default TopBar;

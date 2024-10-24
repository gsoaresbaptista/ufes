import { NavLink, Link, useLocation } from "react-router-dom";
import React, { useState } from "react";

import HomeIcon from "@/components/icons/HomeIcon";
import AboutIcon from "@/components/icons/AboutIcon";
import CodeIcon from "@/components/icons/CodeIcon";
import SettingsIcons from "@/components/icons/SettingsIcon";
import LogoutIcon from "@/components/icons/logoutIcon";

import LogoutDialog from "../LogoutDialog";
import SettingsDialog from "../SettingsDialog";

function SideBarMenu() {
  const [openDialog, setOpenDialog] = useState(false);
  const [openSettings, setOpenSettings] = useState(false);
  const { pathname } = useLocation();
  const liClass = (path: any, item: any) =>
    `sidebar-link group ${path === item.path ? "active" : ""}`;

  const menuItems = [
    { path: "/chat", label: "Home", icon: HomeIcon },
    { path: "/about", label: "Sobre", icon: AboutIcon },
    { path: "/code", label: "Código", icon: CodeIcon },
    {
      path: "/settings",
      label: "Configuração",
      icon: SettingsIcons,
      onClick: (event: any) => {
        event.preventDefault();
        setOpenSettings(!openSettings);
      },
    },
  ];

  return (
    <>
      <LogoutDialog open={openDialog} setOpen={setOpenDialog} />
      <SettingsDialog open={openSettings} setOpen={setOpenSettings} />
      <ul className="flex flex-1 md:flex-col gap-6 justify-around md:justify-start max-w-full">
        {menuItems.map((item, index) => (
          <li key={index} className={liClass(pathname, item)}>
            <NavLink
              to={item.path}
              className="navlink font-medium"
              onClick={item.onClick}
            >
              {React.createElement(item.icon, {
                className: `${
                  pathname === item.path ? "fill-white" : "fill-primary"
                }`,
              })}
              <p>{item.label}</p>
            </NavLink>
          </li>
        ))}
      </ul>
      <div className="sidebar-link group hidden md:block">
        <Link
          to="#"
          className="navlink font-medium"
          onClick={(event) => {
            setOpenDialog(!openDialog);
          }}
        >
          <LogoutIcon className="fill-primary" />
          Deslogar
        </Link>
      </div>
    </>
  );
}

export default SideBarMenu;

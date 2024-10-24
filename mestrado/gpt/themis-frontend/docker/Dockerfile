FROM nginx
COPY ./dist /usr/share/nginx/html
COPY ./docker/nginx.conf /etc/nginx/conf.d/default.conf
CMD ["systemctl", "restart", "nginx"]
EXPOSE 80
CMD ["nginx","-g","daemon off;"]

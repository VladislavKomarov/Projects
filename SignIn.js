import React from 'react';
import Avatar from '@material-ui/core/Avatar';
import Button from '@material-ui/core/Button';
import CssBaseline from '@material-ui/core/CssBaseline';
import TextField from '@material-ui/core/TextField';
import FormControlLabel from '@material-ui/core/FormControlLabel';
import Checkbox from '@material-ui/core/Checkbox';
import Link from '@material-ui/core/Link';
import Grid from '@material-ui/core/Grid';
import Box from '@material-ui/core/Box';
import LockOutlinedIcon from '@material-ui/icons/LockOutlined';
import Typography from '@material-ui/core/Typography';
import { makeStyles } from '@material-ui/core/styles';
import Container from '@material-ui/core/Container';
import { convertLegacyProps } from 'antd/lib/button/button';

import GoogleImage from './images/GoogleIconImage.png'
import FacebookIconImage from './images/FacebookIconImage.png'
import VKIconImage from './images/VKIconImage.png'
import { useStyles } from './SignInStyles';

export default function SignIn() {
  const classes = useStyles();

  return (
    <Box className={classes.mountainImageStyle}>
      <Box className={classes.loginBoxStyle}>
        <Container component="main" className={classes.paperStyle}>
          <Box className={classes.logoStyle}>
          </Box>
          <Typography className={classes.textStyle1} >
            ЛИЧНЫЙ КАБИНЕТ
          </Typography>
          <Typography className={classes.textStyle2}>
            ГОРНОЛЫЖНОГО КУРОРТА
          </Typography>
          <form className={classes.formStyle} noValidate>
            <TextField
              InputProps={{ disableUnderline: true}}
              margin="normal"
              required
              fullWidth
              id="name"
              label="Имя"
              name="name"
              className={classes.inputStyle}
            />
            <TextField
              InputProps={{ disableUnderline: true}}
              margin="normal"
              required
              fullWidth
              name="password"
              label="Пароль"
              type="password"
              id="password"
              className={classes.inputStyle}
            />
            <Button
              type="submit"
              fullWidth
              variant="contained"
              color="primary"
              className={classes.submitStyle}
            >
              Войти
            </Button>
          </form>
          <Grid container justifyContent="center" spacing={0.5}>
            <Grid item>
              <Avatar className={classes.avatarStyle1} src={GoogleImage} onclick="console.log(dd)">
              </Avatar>
            </Grid>
            <Grid item>
              <Avatar className={classes.avatarStyle1} src={FacebookIconImage}>
              </Avatar>
            </Grid>
            <Grid item>
              <Avatar className={classes.avatarStyle2} src={VKIconImage} >
              </Avatar>
            </Grid>
          </Grid>
          <Link href="#" className={classes.registrationLinkStyle} underline="always">
            {"Зарегестрироваться"}
          </Link>
        </Container>
      </Box>
      <Typography className={classes.textStyle3}>
        (с) 2021. Все права защищены
      </Typography>
    </Box>
  );
}
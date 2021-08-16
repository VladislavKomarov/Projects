import { makeStyles } from '@material-ui/core/styles';

import mountainImage from './images/Mountain2.png'
import logoImage from './images/Logo.png'

export const useStyles = makeStyles((theme) => ({
    mountainImageStyle: {
      position: "relative",
      alignItems: "center",
      display: "flex",
      flexDirection: "column",
      height: window.innerHeight,
      "&::before": {
        content: "''",
        display: "flex",
        position: "absolute",
        backgroundRepeat: "no-repeat",
        backgroundPosition: "center",
        backgroundSize: "cover",
        width:"100%",
        minHeight:"610px",
        height:"100%",
        backgroundImage: `url(${mountainImage})`,
        filter: "blur(5px)"
      }
    },
    loginBoxStyle: {
      marginTop: theme.spacing(1),
      display: "flex",
      position: "relative",
      flexDirection: "column",
      alignItems: "center",
      backgroundColor: "#FFFFFF",
      borderRadius: "12px",
      width: "493px",
      height: "550px"
    },
    paperStyle: {
      marginTop: theme.spacing(1),
      display: "flex",
      flexDirection: "column",
      alignItems: "center",
    },
    logoStyle: {
      margin: theme.spacing(1),
      marginTop: theme.spacing(5),
      backgroundImage: `url(${logoImage})` ,
      backgroundRepeat: "no-repeat",
      backgroundPosition: "center",
      height: "69px",
      width: "77px"
    },
    formStyle: {
      width: "359px", 
      marginTop: theme.spacing(1),
      display: "flex",
      flexDirection: "column",
      alignItems: "center",
    },
    inputStyle: {
      background: "#F3F3F3",
      borderRadius: "10px",
      paddingLeft: theme.spacing(2),
      "& .MuiFormLabel-root": {
        color: "#D3D3D3",
        fontFamily: "Roboto",
        fontStyle: "normal",
        fontWeight: "normal",
        fontSize: "10px",
        marginLeft: theme.spacing(2),
      },
    },
    submitStyle: {
      margin: theme.spacing(3, 0, 2),
      background: "#4158F6",
      border: "1px solid #4158F6",
      boxSizing: "border-box",
      borderRadius: "21px",
      width: "169px",
      height: "46px",
      "& .MuiButton-label": {
        fontSize: "12px",
        fontFamily: "Roboto",
        fontStyle: "normal",
        textTransform: "capitalize"
      },
    },
    textStyle1: {
      fontFamily: "Roboto",
      fontStyle: "normal",
      fontWeight: "bold",
      fontSize: "14px",
      color: "#4158F6",
      marginTop: theme.spacing(1)
    },
    textStyle2: {
      fontFamily: "Roboto",
      fontStyle: "normal",
      fontWeight: "bold",
      fontSize: "16px",
      color: "#000000",
      marginBottom: theme.spacing(3)
    },
    textStyle3: {
      display: "flex",
      position: "relative",
      fontFamily: "Roboto",
      fontStyle: "normal",
      fontWeight: "normal",
      fontSize: "9px",
      lineHeight: "24px",
      color: "#000000",
      marginTop: theme.spacing(3)
    },
    avatarStyle1: {
      margin: theme.spacing(1),
      border: "1px solid #DDDDDD",
      boxSizing: "border-box",
      backgroundColor: "inherit",
      "& .MuiAvatar-img": {
        width: "21px",
        height: "21px"
      }
    },
    avatarStyle2: {
      margin: theme.spacing(1),
      border: "1px solid #DDDDDD",
      boxSizing: "border-box",
      backgroundColor: "inherit",
      "& .MuiAvatar-img": {
        width: "30px",
        height: "17px"
      }
    },
    registrationLinkStyle: {
      margin: theme.spacing(1),
      fontFamily: "Roboto",
      fontStyle: "normal",
      fontWeight: "normal",
      fontSize: "12px",
      lineHeight: "14px",
      color: "#1371FA"
    }
}));